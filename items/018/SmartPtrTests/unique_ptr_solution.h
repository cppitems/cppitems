#pragma once
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

template <typename T> class unique_ptr {
  T *ptr;
  std::function<void(T *)> del;  

public:
  // construction 
  unique_ptr(T *p) : ptr(p), del([](auto *ptr) { delete ptr; }) {}

  // TODO: constructor taking custom deleter (TestE and TestF)
  template <typename DEL>
  unique_ptr(T *p, DEL &&del) : ptr(p), del(std::forward<DEL>(del)) {}

  // TODO: move constuctor (TestA)
  unique_ptr(unique_ptr &&other)
      : ptr(other.ptr), del(std::forward<decltype(other.del)>(other.del)) {
    other.ptr = nullptr;
  };

  // TODO: move assignment operator (TestB)
  unique_ptr &operator=(unique_ptr &&other) {
    reset();
    this->ptr = other.ptr;
    this->del = std::move(other.del);
    other.ptr = nullptr;
    other.reset();
    return *this;
  };
  // TODO: assignment operator from nullptr (TestB)
  unique_ptr &operator=(std::nullptr_t) {
    reset();
    return *this;
  }

  // TODO: 'release' -> stop management of resource and return pointer (TestC)
  T *release() {
    auto tmp = ptr;
    ptr = nullptr;
    return tmp;
  }

  // TODO: 'reset' -> reset managed resource (TestC)
  void reset(T *other = nullptr) {
    if (ptr) {
      del(ptr);
    }
    this->ptr = other;
  }

  // destruction
  ~unique_ptr() { reset(); }

  // access
  T *get() { return ptr; }
  T &operator*() { return *get(); }
  T *operator->() { return get(); }

  // TODO: comparisons (TestD)
  bool operator==(std::nullptr_t) { return ptr == nullptr; }
  bool operator!=(std::nullptr_t) { return ptr != nullptr; }
  bool operator==(const unique_ptr &other) { return ptr == other.ptr; }
  bool operator!=(const unique_ptr &other) { return ptr != other.ptr; }
};
