9 // item status
# Smart Pointers

Raw pointers are a versatile tool in C. They are used for
- (a) cheaply passing an object for inspection purposes (no modification) 
```pmans
void func(/*b*/ const Widget * /*x*/w) {
    // can inspect Widget, but cannot modify
    // no pointer arithmetic (useless)
}
``` 
- (b) cheaply passing an object intended to be modified by the callee
```pmans
void func(/*b*/ Widget * /*x*/w) {
    // can modify Widget
    // no pointer arithmetic (useless)
}
``` 
- (c) passing arrays (together with a size/length)
```pmans
void func(/*b*/ const Widget * /*x*/array, /*b*/ int /*x*/ size) {
    // pointer arithmetic to access array contents
}
``` 
- (d) handles to objects created with dynamic storage duration (or other resources with similar needs)
```pmans
/*b*/ Widget * /*x*/ptr = /*f*/ new /*x*/ Widget(); // object creation, lifetime decoupled from local scope
ptr->member // access object
func(ptr);  // pass into function
/*f*/ delete /*x*/ ptr; // manual termination required

/*b*/ Widget * /*x*/get_widget(){ 
    return /*f*/ new /*x*/ Widget(); // return from a function
}
``` 
> What could be used in C++ for the situations above?

Further let's list some functionality we expect to work for a raw pointer:
```pmans
struct Widget{
    int m;
};
...
  Widget w;
  Widget *ptr /*b1*/ = &w;
  Widget *ptr2 /*b1*/ = ptr;
  ptr2 /*b1*/ = &w;
  ptr/*b2*/ ->m = 5;
  (/*b1*/ *ptr).m = 6;
  /*b2*/ ++ptr;
  /*b2*/ --ptr;
  if (ptr /*b2*/ != &w) {throw;};
  ptr /*b1*/ = nullptr;
  if (ptr /*b2*/ == &w) {throw;};
  Widget *ptr /*b2*/ = /*f3*/ new Widget{};
```
The *smart pointers* from the standard library discussed in the following aim to follow this functionality as long as it does not contradict to the underlying idea of the respective *smart pointer*.

## std::unique_ptr
In cases where an exclusive ownership of a resource is desired, `std::unique_ptr` provides (and demands) expressiveness and delivers some convenience.
Let's consider the function `get_widget` above, which returns a raw pointer to a `Widget` in this snippet:
```pmans
struct Widget {
    int m;
}
...
Widget */*b3*/ ptr = get_widget();
// is /*f3*/ ptr a resource which needs to be be release?
// who will release the resource?
```
Or let's consider s slight variation of the function signature:
```pmans
Widget w;
Widget */*b3*/ ptr = get_widget(&w);
// same questions
```
Let's transform `get_widget` to construct and return a `std::unique_ptr` instead
```pmans
auto get_widget2() {
  return /*b*/ std::unique_ptr /*x*/<Widget>(new Widget{}); // (1) passing pointer directly (here using new)
  return /*b*/ std::make_unique /*x*/<Widget>(); // (2) using public constructors indirectly
}
```
Now the return type is expressive (it is clear the caller becomes the owner of the returned object) and access works the same way as for a raw pointer:
```pmans
  /*b*/ std::unique_ptr /*x*/<Widget> w2 = get_widget2();
  w2->m = 5; // same as for a raw pointer
  (*w2).m = 10; // same as for a raw pointer
  // no manual termination of lifetime required
```
As we can see, the returned object is usable like a raw pointer w.r.t. to accessing the pointed-to-object.
Additionally, manual termination is not required: the owned resource is released when the object goes out of scope.

> Which further restrictions (SMFs) can we expect if the `std::unique_ptr` wants to model a unique handle?

> How is the automatic release of the resource implemented?

> What about pointer arithmetic and other operators?

### Overhead: how does a std::unique_ptr look like?
If we decide to use a `std::unique_ptr` instead of a raw pointer, what can we expect in terms of performance? 
To argue about this, let's look how (a small part of) of a simplified implementation looks like:
```pmans
template <typename T> class /*f*/ unique_ptr /*x*/ {
  T */*b*/ ptr /*x*/ = nullptr;

public:
  /*f*/ unique_ptr /*x*/(T *p) : ptr(p) {}
  ~/*f*/ unique_ptr /*x*/() { delete /*b*/ ptr /*x*/; }
  T &operator*() { return */*b*/ ptr /*x*/; }
  T *operator->() { return /*b*/ ptr /*x*/; }
  ...
};
template <typename T, typename... ARGS>
/*f*/ unique_ptr /*x*/<T> /*b*/ make_unique /*x*/(ARGS &&... args) {
  return /*f*/ unique_ptr /*x*/<T>(/*b*/ new /*x*/ T(std::forward<ARGS>(args)...));
}
```
We can see that it is a thin wrapper class with a single raw pointer member `ptr` which is initialized from the passed pointer. For this we can expect the memory overhead to be zero (if no custom deleter is used). 
Construction has a minimal overhead and access and destruction have no overhead (when compiling with optimizations).

### Custom deleter example

If a `unique_ptr` manages a resource which is not a dynamically allocated object (for which the default release mechanism is fine, e.g., `delete` or `delete[]`) a custom deleter can be passed at construction. Let's look at a small example which uses a file handle:
```pmans
void parse(FILE *handle){}; // parsing the file

int main() {
  auto filename = "data.json";
  auto mode = "r";
  FILE *handle = /*f*/ std::fopen /*x*/(filename, mode);
  if (handle) {
    parse(handle);
    /*f*/ std::fclose /*x*/(handle);
  }
}
```
If we would like to wrap the file handle obtained from `fopen` using a `unique_ptr` the default release mechanism `delete` does not apply but instead `fclose` is required to be called on the handle. This can be achieved using custom function object as deleter:
```pmans
auto /*b*/ open /*x*/(const char *name, const char *mode) {
  auto closer = [](FILE *handle) { /*f*/ std::close /*x*/(handle); };
  return std::unique_ptr<FILE, decltype(closer)>(/*f*/ std::fopen /*x*/(name, mode), closer);
};

int main() {
  auto filename = "data.json";
  auto mode = "r";
  auto file = open(filename, mode);
  if (file) {
    parse(file.get()); // pass raw pointer to function
    // parse(&*file); // more cumbersome, this is why get is there
  }
}
```
This demonstrates that a `unique_ptr` can also be used to manage resources not directly created with `new` and released with `delete`.

## std::shared_ptr
If a resource is intended to be shared (i.e., multiple handles exist simultaneously) and are shared between participating entities it is not straightforward to decide when to perform the final release of the resource: is is desired that is only happens after all participating entities are no more able to access the resource.
The `std::shared_ptr` solves this problem using *reference counting*: the number of valid handles (references) to a resource, are tracked using a single counter variable per resource: 
- on construction of the first (original) handle the counter is set to one
- whenever an additional handle is constructed the counter is incremented
- whenever a handle is invalidated the counter is decremented
- if the counter is decremented to zero the resource is released (as no more handles are present)

How to obtain a `shared_ptr`? Let's look at some examples:
```pmans
std::shared_ptr<Widget> sp1 = std::make_shared<Widget>(); // using make_shared
std::shared_ptr<Widget> sp2 = std::shared_ptr<Widget>(new Widget{}) // ctor + new
std::unique_ptr<Widget> up1 = get_widget2(); // obtain up from function
std::shared_ptr<Widget> sp3 = get_widget2(); // obtain sp (moving from rvalue up)
std::shared_ptr<Widget> sp4 = std::move(up1); // obtain sp (moving from lvalue up)
// state of up1 here?
// what is the reference count of sp1, sp2, sp2, and sp4 here?
```
> Which restrictions can we expect to be lifted for `std::shared_ptr` (compared to `std::unique_ptr`)?

> Does principle of the automatic release mechanism differ from `std::unique_ptr`?

> What about pointer arithmetic and other operators?

### Overhead: how does a `std::shared_ptr` look like
If we decide to use a `std::shared_ptr` instead of a `std::unique_ptr` or raw pointer, what can we expect in terms of performance? To argue about this, let's look again at how (a small part of) of a simplified implementation looks like:
```pmans
template <class T> class /*f*/ shared_ptr /*x*/ {
  struct /*f*/ ControlBlock /*x*/ {
    unsigned count = 1;
  };
  /*b*/ ControlBlock *cb /*x*/;
  /*b*/ T *ptr /*x*/;

  void /*f*/ increment /*x*/() {
    if (cb)
      ++cb->count;
  }
  void /*f*/ decrement /*x*/() {
    if (cb && --cb->count == 0) {
      delete ptr;
      delete cb;
    }
  }

public:
  /*f*/ shared_ptr /*x*/(T *ptr) : cb(new ControlBlock()), ptr(ptr) {}
  /*f*/ shared_ptr /*x*/(const /*f*/ shared_ptr /*x*/ &other) : cb(other.cb), ptr(other.ptr) {
    // copy ctor
  }
  /*f*/ shared_ptr /*x*/(/*f*/ shared_ptr /*x*/ &&other) : cb(other.cb), ptr(other.ptr) {
    // move ctor
  }
  /*f*/ shared_ptr /*x*/ &operator=(const /*f*/ shared_ptr /*x*/ &other) {
    // copy-assign
    return *this;
  }
  ~/*f*/ shared_ptr /*x*/() { 
    // dtor; 
  }
  ...
  T *operator->() const { return ptr; }
  T &operator*() const { return *ptr; }
};
template <typename T, typename... ARGS>
/*f*/ shared_ptr /*x*/<T> make_shared(ARGS &&... args) {
  return /*f*/ shared_ptr /*x*/<T>(new T(std::forward<ARGS>(args)...));
}

```
It is visible that the construction triggers a subsequent dynamic allocation for the `ControlBlock`. The obtained pointer is stored as member `cb` in the `shared_pointer`, additionally to the pointer of the managed object `ptr`. Additionally, it is apparent that many of the SMFs will need to access the `ControlBlock`.
This means a `shared_ptr` does introduce some overhead (memory and access). This has to be considered for practical applications: if very small objects are managed by a `shared_ptr`, the relative increase of the memory footprint is not negligible. It might still be practical to use a `shared_ptr` for small objects, depending on the application context.

> What is expected to happen in the body of the three SMFs marked above (to implement the reference counting)?

> Why is the `ControlBlock` a structure and not simply a integral type? Would a static member be sufficient, too?

As we can see from the snippet above `shared_ptr`s can be copied and moved.
Let's examine some examples using again a `Widget` and a function returning a `share_ptr<Widget>`:
```pmans
struct Widget {
  int m;
};

auto get_widget() {
  return std::make_shared<Widget>(); // (2) using public constructors indirectly
}
```
**Checking on the reference count**
```pmans
  auto /*f2*/ sp = get_widget(); 
  std::cout << /*f2*/ sp.use_count() << std::endl;
  auto /*f3*/ sp2 = /*f2*/ sp; // copy ctor
  std::cout << /*f2*/ sp.use_count() << std::endl;
  std::cout << /*f3*/ sp2.use_count() << std::endl;
  std::cout << (/*f3*/ sp2 == /*f2*/ sp) << std::endl;
  {
      auto /*f3*/ sp3 = /*f2*/ sp; // copy ctor
      std::cout << /*f2*/ sp.use_count() << std::endl;
  }
  std::cout << /*f2*/ sp.use_count() << std::endl;
```

> What are the reference counts in the above snippet?

**Capturing the managed object outside**
```pmans
  Widget *ptr = nullptr;
  {
    auto sp = get_widget();
    ptr = sp.get();
  }
  ptr->m = 5; // is this ok?
```
> Is it safe to perform the last line of the snippet above?

**Managing the same resource more than once**
```pmans
  auto sp1 = get_widget();
  auto sp2 = get_widget();
  std::shared_ptr<Widget> sp3(sp1);
  {
    // is this ok?
    std::shared_ptr<Widget> sp4(sp2.get());
  }
```
> What does it mean if a resource is managed "more than once"?

**Providing a `shared_ptr` from "inside" #1**
```pmans
struct Widget2 {
  int m;
  auto mfunc() { 
      return std::shared_ptr<Widget2>(this); // (1) is this ok?
  }
};

auto get_widget2() {
  return std::make_shared<Widget2>(); 
}
  ...
  auto sp = get_widget2();
  auto sp2 = sp->mfunc(); // (2) is this ok?
```
> Is there a way to return a `shared_ptr` from within a managed class?

**Providing a `shared_ptr` from "inside" #2**

To overcome the problem in the previous snippet a solution is to inherit from `std::enable_shared_from_this`:
```pmans
struct Widget3 : std::enable_shared_from_this<Widget3> {
  int m;
  auto mfunc() { 
      return shared_from_this(); // is this ok now?
  }
};

auto get_widget3() { return std::make_shared<Widget3>(); }

int main() {
  // return from inside
  auto sp = get_widget3();
  auto sp2 = sp->mfunc(); // is this ok now?
}
```
This adds a member of type `std::weak_ptr<Widget3>` to the structure which is connected to the managing `shared_ptr` instance on construction of the `shared_ptr`

### std::weak_ptr 
A `std::weak_ptr` can be imagined to be a non-owning *potential* `shared_ptr` associated with a managing `shared_ptr`: it does not participate in the reference counting (actually it has its own reference counter for weak references). A `weak_ptr` does not influence the destruction of an object managed by a `shared_ptr` once the reference count goes to zero.
Nevertheless: a `weak_ptr` can be converted to a `shared_ptr`:
```pmans
  std::weak_ptr<Widget> wp;
  {
    auto sp = std::make_shared<Widget>();
    wp = sp;
    auto sp2 = wp.lock();
  }
  if(!wp.expired()){
     auto sp = wp.lock();
     sp->m = 5;
  }
```
This makes is suitable to be used in conjunction with `std::enable_shared_from_this`: it will simply provide a `shared_ptr` to the object via `shared_from_this()`.

### Thread safety

> Is a `share_ptr` "thread-safe"?


## Links

- `std::unique_ptr`: https://en.cppreference.com/w/cpp/memory/unique_ptr
- `std::shared_ptr`: https://en.cppreference.com/w/cpp/memory/shared_ptr
- `std::weak_ptr`: https://en.cppreference.com/w/cpp/memory/weak_ptr'
- `std::enable_shared_from_this`: https://en.cppreference.com/w/cpp/memory/enable_shared_from_this
- CPPCON2019 talk on smart pointers: https://www.youtube.com/watch?v=xGDLkt-jBJ4
- Blog entry on `std::make_unique`: https://abseil.io/tips/126
- Scott Meyers, Effective Modern C++: Items 18-21 on smart pointers