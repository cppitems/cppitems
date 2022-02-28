1 // item status
# Exercise EX3.2

The exercise is available as a git repository at:
https://github.com/cppitems/ex3.2

# Possible solutions

## Task1: Solution w/o custom deleter
```pmans
template <typename T> 
class unique_ptr {
public:
  using pointer = T *;
  using element_type = T;
  T *get() { return ptr; }
  // unique_ptr(T *resource) handles assignment from nullptr implicitly
  unique_ptr(T *resource) : ptr(resource) {}
  unique_ptr(const unique_ptr &other) = delete;
  unique_ptr &operator=(const unique_ptr &other) = delete;
  unique_ptr(unique_ptr &&other) { std::swap(ptr, other.ptr); };
  unique_ptr &operator=(unique_ptr &&other) {
    reset(other.release());
    return *this;
  };
  ~unique_ptr() { delete ptr; }
  void reset(T *resource = nullptr) {
    delete ptr;
    ptr = resource;
  }
  T *release() {
    auto res = ptr;
    ptr = nullptr;
    return res;
  }
  // handles comparison to nullptr implicitly via converting constructor
  bool operator==(const unique_ptr &other) { return (ptr == other.ptr); }
  // handles comparison to nullptr implicitly via converting constructor
  bool operator!=(const unique_ptr &other) { return ptr != other.ptr; }
private:
  T *ptr = nullptr;
};
```


## Task2: Solution w/ custom deleter
```pmans
template <typename T, /*b*/ typename DELETER = default_deleter<T> /*x*/> 
class unique_ptr {
public:
  using pointer = T *;
  using element_type = T;
  T *get() { return ptr; }
  unique_ptr(T *resource) : ptr(resource) {}
  unique_ptr(T *resource, /*b*/ const DELETER& del /*x*/) : ptr(resource), /*b*/ del(del) /*x*/ {}
  unique_ptr(const unique_ptr &other) = delete;
  unique_ptr &operator=(const unique_ptr &other) = delete;
  unique_ptr(unique_ptr &&other) : /*b*/ del(std::move(other.del)) /*x*/ {
    std::swap(ptr, other.ptr);
  };
  unique_ptr &operator=(unique_ptr &&other) {
    reset(other.release());
    /*b*/ del = std::move(other.del); /*x*/ // will fallback to copy in C++17
    return *this;
  };
  unique_ptr &operator=(std::nullptr_t) {
    reset();
    return *this;
  };
  ~unique_ptr() {
    // delete ptr;
    if (ptr) { // required to avoid deletion when not managing a resource
      /*b*/ del(ptr) /*x*/;
    }
  }
  void reset(T *resource = nullptr) {
    // delete ptr;
    if (ptr) { // required to avoid deletion when not managing a resource
      /*b*/ del(ptr) /*x*/;
    }
    ptr = resource;
  }
  T *release() {
    auto res = ptr;
    ptr = nullptr;
    return res;
  }
  bool operator==(const unique_ptr &other) { return (ptr == other.ptr); }
  bool operator!=(const unique_ptr &other) { return ptr != other.ptr; }
  // these take precedence over the ones above which rely on implicit conversion
  bool operator!=(std::nullptr_t) { return ptr != nullptr; }
  bool operator==(std::nullptr_t) { return ptr == nullptr; }

private:
  T *ptr = nullptr;
  /*b*/ DELETER del /*x*/;
};
```
