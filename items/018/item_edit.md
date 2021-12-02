1 // item status
# Smart Pointers

Raw/native pointers are a versatile tool in C. 
They are used for
- (a) cheaply passing an object for inspection purposes (no modification) 
<!--  // void func(/*f*/ const Widget & /*x*/w); // c++ -->
```pmans
void func(/*b*/ const Widget * /*x*/w) {
    // can inspect Widget, but cannot modify
    // no pointer arithmetic (allowed but useless)
}
``` 
- (b) cheaply passing an object intended to be modified by the callee
<!-- // void func(/*f*/ Widget & /*x*/w); // c++ -->
```pmans
void func(/*b*/ Widget * /*x*/w) {
    // can modify Widget
    // no pointer arithmetic (allowed but useless)
}
``` 
- (c) passing arrays (together with a size/length)
<!-- // void func(/*f*/ std::vector<const Widget> & /*x*/array); // c++ -->
```pmans
void func(/*b*/ const Widget * /*x*/array, /*b*/ int /*x*/ size) {
    // pointer arithmetic to access array contents
}
``` 
- (d) passing handles to objects created with dynamic storage duration (or other resources with similar needs)
<!-- // c++ solution here? -> smart pointer -->
```pmans
/*b*/ Widget * /*x*/ptr = /*f*/ new /*x*/ Widget(); // object creation, dynamic lifetime
func(ptr);  // pass into function
/*f*/ delete /*x*/ ptr; // manual termination required

/*b*/ Widget * /*x*/get_widget(){ 
    return /*f*/ new /*x*/ Widget(); // return from a function
}
``` 
> What could be used in C++ for the situations above?
<!-- 
> - (a)-(c) see comments in snippets above 

> - (d) a smart pointer, e.g., `std::unique_ptr` or `std::shared_ptr`, is the go-to solution to manage  dynamically allocated resources in c++
-->

Further let's list some functionality we expect to work for a raw pointer:
```pmans
struct Widget{
    int m;
};
...
  Widget w; 
  Widget *ptr /*b1*/ = &w;              // (1) init/construction
  Widget *ptr2 /*b1*/ = ptr;            // (2) init/construction
  ptr2 /*b1*/ = &w;                     // (3) assign
  ptr/*b2*/ ->m = 5;                    // (4) member access using ->
  (/*b1*/ *ptr).m = 6;                  // (5) dereferencing + .
  /*b2*/ ++ptr;                         // (6) increment
  /*b2*/ --ptr;                         // (7) decrement
  if (ptr /*b2*/ != &w) {throw;};       // (8) operator!=
  ptr /*b1*/ = nullptr;                 // (9) nullable
  if (ptr /*b2*/ == &w) {throw;};       // (10a) operator==
  if (ptr /*b2*/ != &w) {throw;};       // (10b) operator!=
  Widget *ptr /*b2*/ = /*f3*/ new Widget{};    // (11) handle to a dynamic resource
  Widget/*b1*/ [/*b1*/ ] array = /*f3*/ new Widget/*b1*/ [10/*b1*/ ]{};    // (12a) array 
  array/*b1*/ [9/*b1*/ ] = Widget{9};                 // (12b) indexed array access
```
The *smart pointers* from the standard library discussed in the following aim to follow this functionality as long as it does not stand against the underlying idea of the respective *smart pointer*.

## std::unique_ptr
In cases where an exclusive ownership of a resource is desired, `std::unique_ptr` provides (and demands) expressiveness and delivers some convenience.
Let's consider the function `get_widget` above, which returns a raw pointer to a `Widget` in this snippet:
```pmans
struct Widget {
    int m;
}
... 
Widget */*b3*/ ptr = get_widget();
// is /*f3*/ ptr a resource which needs to be be released?
// who will release the resource?
```
Or let's consider a slight variation of the function signature:
```pmans
Widget w;
Widget */*b3*/ ptr = get_widget(&w);
// same questions
```
Let's transform `get_widget` to construct and return a `std::unique_ptr` instead
```pmans
auto get_widget2() {
  // (1) passing pointer directly (here using new)
  return /*b*/ std::unique_ptr /*x*/<Widget>(new Widget{});
  // (2) using constructors of Widget indirectly (by forwarding the arguments) 
  return /*b*/ std::make_unique /*x*/<Widget>(); 
}
```
Now the return type is expressive (it is clear the caller becomes the owner of the returned object) and access works the same way as for a raw pointer:
```pmans
  /*b*/ std::unique_ptr /*x*/<Widget> smart = get_widget2();
  smart->m = 5;     // same as for a raw pointer
  (*smart).m = 10;  // same as for a raw pointer
  // no manual termination of lifetime required:
  // destruction happens when local variable 'smart' goes out of scope
```
As we can see, the returned object is usable like a raw pointer w.r.t. to accessing the pointed-to object.
Additionally, manual termination is not required: the owned resource is released when the smart pointer variable goes out of scope.

> Which further restrictions (SMFs) can we expect if the `std::unique_ptr` wants to model a **unique** handle to a resource?
>```pmans
>std::unique_ptr<Widget> w2 = get_widget2(); // ctor, ok
>w2 = w3;                         // copy assign, ??
>std::unique_ptr<Widget> w4 = w2; // copy ctor, ??
>std::unique_ptr<Widget> w3 = std::move(w2); // move ctor, ??
>w2 = std::move(w3));                        // move assign, ??
>```

> How is the automatic release of the resource implemented?
<!-- > - in the destructor of `unique_ptr` -->

> What about pointer arithmetic and other operators? Are they supported?
> - arithmetic, e.g., `operator++`, `operator--` ?? 
> - comparison, e.g., `opterator==`, `opterator!=` ?? 

### Overhead: what does a `std::unique_ptr` look like?
If we decide to use a `std::unique_ptr` instead of a raw pointer, what can we expect in terms of performance? 
To argue about this, let's look what (a small part of) a simplified implementation looks like:
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
/*f*/ unique_ptr /*x*/<T> /*b*/ make_unique /*x*/(ARGS&&... args) {
  return /*f*/ unique_ptr /*x*/<T>(/*b*/ new /*x*/ T(std::forward<ARGS>(args)...));
}
```
We can see that it is a thin wrapper class with a single raw pointer member `ptr` which is initialized from the passed pointer. For this we can expect the memory overhead to be zero. 
Construction has a minimal overhead;
 access and destruction have no overhead (when compiling with optimizations).

### Custom deleter example

If a `std::unique_ptr` manages a resource which is not a dynamically allocated object (for which the default release mechanism is applicable, i.e., `delete` or `delete[]`) a custom deleter can be passed at construction. Let's look at a small example which uses a file handle:
```pmans
void parse(FILE *handle){}; // parsing a file

int main() {
  auto filename = "data.json";
  auto mode = "r";
  FILE *handle = /*f*/ std::fopen /*x*/(filename, mode); // obtain resource, i.e., a custom 'new'
  if (handle) {
    parse(handle);
    /*f*/ std::fclose /*x*/(handle); // release resource, i.e., a custom 'delete'
  }
}
```
If we would like to wrap the file handle obtained from `fopen` using a `std::unique_ptr` the default release mechanism `delete` does not apply but instead `fclose` is required to be called on the handle. This can be achieved by using a custom function object as deleter:
```pmans
auto /*b*/ open /*x*/(const char *name, const char *mode) {
  auto /*b7*/ deleter = [](FILE *handle) { /*f*/ std::fclose /*x*/(handle); };
  return std::unique_ptr<FILE, decltype(closer)>(/*f*/ std::fopen /*x*/(name, mode), /*b7*/ deleter);
};

int main() {
  auto filename = "data.json";
  auto mode = "r";
  auto file = open(filename, mode);
  if (file) {
    parse(file); // (1) does this work like this?
  }
}
```
> Is there an implicit conversion from `std::unique_ptr` to the underlying raw pointer (see (1) in the snippet above) ?
<!-- 
    parse(file.get()); // how to pass raw pointer here now ?
    // parse(&*file); // more cumbersome, this is why get is there
-->
This demonstrates that a `std::unique_ptr` can also be used to manage resources which are not (directly) created with `new` and released with `delete`.

#### Managing an array
When a `std::unique_ptr` is used to manage an array, indexed access is provided via the `[]` operator:
```pmans
std::unique_ptr<double/*b2*/ []> up(new double/*b3*/ [10]);
up/*b1*/ [9/*b1*/ ] = ...; // set last element of array
```
The relase of the array (which now requires `delete[]`) is realized by an appropriate deleter in the template specialization of `std::unique_ptr`.



## std::shared_ptr
If a resource is intended to be shared (i.e., multiple handles exist simultaneously and are shared between participating entities) it is not straightforward to decide when to perform the final release of the resource: is is desired that this only happens after all participating entities are no more able to access the resource.
The `std::shared_ptr` solves this problem using *reference counting*: the number of valid handles (references) to a resource are tracked using a single counter variable per resource: 
- on construction of the first (original) handle the counter is set to '1'
- whenever an additional handle is constructed the counter is incremented
- whenever a handle is invalidated the counter is decremented
- if the counter is decremented to '0' the resource is released (as no more handles are present)

How to obtain a `shared_ptr`? Let's look at some examples:
```pmans
std::shared_ptr<Widget> sp1 = std::make_shared<Widget>(); // using make_shared
std::shared_ptr<Widget> sp2 = std::shared_ptr<Widget>(new Widget{}) // ctor + new
std::unique_ptr<Widget> up1 = get_widget2(); // unique_ptr
std::shared_ptr<Widget> sp3 = std::move(up1); // from rvalue unique_ptr
std::shared_ptr<Widget> sp4 = get_widget2(); // implicit from rvalue unique_ptr

// state of up1 here? 
// what is the reference count of sp1, sp2, sp3, and sp4 here? 
```
> Is it possible to convert between `std::unique_ptr` and `std::shared_ptr`?
<!-- 
> - yes, an implicit conversion is available from `unique_ptr` to `shared_ptr` (from rvalues only)

> - a conversion from `shared_ptr` to `unique_ptr` is not allowed (would need to invalidate all references)
-->

> Which restrictions (SMFs) can we expect to be lifted for `std::shared_ptr` (compared to `std::unique_ptr`)?
<!--
> - copy assignment and copy construction is expected to be available
-->

> Does the principle of the automatic release mechanism differ from `std::unique_ptr`?
<!--
> - identical in general; but the condition for its execution differs: only a decrement of the reference count to '0' triggers a release of the resource;

> - this might also happen during a copy assignment, not only when a variable goes out of scope
-->

> What about pointer arithmetic and other operators?
<!--
> - same as for `std::unique_ptr`
-->


### Overhead: what does a `std::shared_ptr` look like
If we decide to use a `std::shared_ptr` instead of a `std::unique_ptr` or a raw pointer, what can we expect in terms of performance? To argue about this, let's look again at what (a small part of) a simplified implementation looks like:
```pmans
template <class T> class /*f*/ shared_ptr /*x*/ {
  struct /*f*/ ControlBlock /*x*/ {
    int count = 1; 
    // some more members in stdlib
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
  /*f*/ shared_ptr /*x*/(T *ptr) : cb(/*b3*/ new ControlBlock()), ptr(ptr) {}
  /*f*/ shared_ptr /*x*/(const /*f*/ shared_ptr /*x*/ &other) : cb(other.cb), ptr(other.ptr) {
    // TODO
  }
  /*f*/ shared_ptr /*x*/(/*f*/ shared_ptr /*x*/ &&other) : cb(other.cb), ptr(other.ptr) {
    // TODO
  }
  /*f*/ shared_ptr /*x*/ &operator=(const /*f*/ shared_ptr /*x*/ &other) {
    // TODO
    return *this;
  }
  ~/*f*/ shared_ptr /*x*/() { 
    // TODO
  }
  ...
  T *operator->() const { return ptr; }
  T &operator*() const { return *ptr; }
};
template <typename T, typename... ARGS>
/*f*/ shared_ptr /*x*/<T> make_shared(ARGS&&... args) {
  return /*f*/ shared_ptr /*x*/<T>(new T(std::forward<ARGS>(args)...));
}

```
It is visible that the construction (of the "original" handle ) triggers a subsequent dynamic allocation for the `ControlBlock`. The obtained pointer is stored as member `cb` in the `shared_pointer` additionally to the pointer of the managed object `ptr`. It is apparent that many of the SMFs will need to access the `ControlBlock`.
This means a `shared_ptr` does introduce some overhead (memory and access). This has to be considered for practical applications: if very small objects are managed by a `shared_ptr`, the relative increase of the memory footprint is not negligible. It might still be practical to use a `shared_ptr` for small objects, depending on the application context.

> What is expected to happen in the body of the three SMFs marked above (to implement the reference counting)?
<!--
> - see comments above
    // copy ctor: increment
    // move ctor: no increment

    // copy assign: 
    // decrement for current resource
    // reassign
    // increment for newly assigned resource
    return *this;

    // dtor: decrement
-->

> Why is the `ControlBlock` a structure and not simply a integral type?
<!--
> - in this simple case, could be just a pointer to integer; in the stdlib, some more members are present in the `ControlBlock` structure to support advanced features of a `shared_ptr`: weak reference counting, custom deleters, and aliasing construction (access with offset)
-->
> Would a static member be sufficient for reference counting, too?
<!--
> - no, a static member could only count references for all handles to shared pointers per type, but not per resource.
-->

As we can see from the snippet above `shared_ptr`s can be copied and moved.
Let's examine some examples using again a `Widget` and a function returning a `share_ptr<Widget>`:
```pmans
struct Widget {
  int m;
};

auto get_widget() {
  return std::make_shared<Widget>(); // using public constructors indirectly
}
```
**Checking on the reference count**
```pmans
  auto /*f2*/ sp = get_widget(); 
  std::cout << /*f2*/ sp.use_count() << std::endl; //  ??
  auto /*f3*/ sp2 = /*f2*/ sp; // copy ctor
  std::cout << /*f2*/ sp.use_count() << std::endl; //  ??
  std::cout << /*f3*/ sp2.use_count() << std::endl; // ??
  std::cout << (/*f3*/ sp2 == /*f2*/ sp) << std::endl; // ??
  {
      auto /*f3*/ sp3 = /*f2*/ sp; // copy ctor 
      std::cout << /*f2*/ sp.use_count() << std::endl; // ??
  }
  std::cout << /*f2*/ sp.use_count() << std::endl; // ??
```

> What are the reference counts in the above snippet?
<!--
> - see inline comments
-->

**Capturing the managed object outside**
```pmans
  Widget *ptr = nullptr;
  {
    auto sp = get_widget(); // (1a)
    ptr = sp.get(); // (2)
  } // (1b)
  ptr->m = 5; // (3) is this ok?
```
> Is it safe to perform the last line of the snippet above?
<!--
> - no, the resource `ptr` is pointing to is deallocated at (1b)
-->


**Managing the same resource more than once**
```pmans
  auto sp1 = get_widget();
  auto sp2 = get_widget();
  std::shared_ptr<Widget> sp3(sp1); // (1) copy ctor
  std::shared_ptr<Widget> sp4(sp2.get()); // (2) ctor from pointer
  // how many control blocks do exist now?
```
> What does it mean if a resource is managed "more than once"?
<!--
> - this means that two independent reference counting control blocks exist for a single shared resource; this situation should never be created
-->

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
<!--
> - returning a `shared_ptr` constructed from `this` leads again to a "double management" (like above)
> - to support such a situation, the object has to know that it is managed by a shared pointer and somehow needs access to the respective control block, see next example
-->

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
This adds a member of type `std::weak_ptr<Widget3>` to the structure which is connected to the managing `shared_ptr` instance on construction of the `shared_ptr` .

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
This makes a `weak_ptr` suitable to be used in conjunction with `std::enable_shared_from_this`: it can provide a `shared_ptr` to the object via `shared_from_this()`.

### Thread safety

> Is a `std::share_ptr` "thread-safe"?
<!--
> - the modifying access to the shared_ptr (i.e., its control block) is thread-safe
> - the thread-safety-ness w.r.t. to the access of the managed resource is not influenced by the shared pointer: the shared pointer does not synchronize modifying access to the resource in any way; the access is direct.
-->


## Links

- `std::unique_ptr`: https://en.cppreference.com/w/cpp/memory/unique_ptr
- `std::shared_ptr`: https://en.cppreference.com/w/cpp/memory/shared_ptr
- `std::weak_ptr`: https://en.cppreference.com/w/cpp/memory/weak_ptr'
- `std::enable_shared_from_this`: https://en.cppreference.com/w/cpp/memory/enable_shared_from_this
- CPPCON2019 talk on smart pointers: https://www.youtube.com/watch?v=xGDLkt-jBJ4
- Blog entry on `std::make_unique`: https://abseil.io/tips/126
- Scott Meyers, Effective Modern C++: Items 18-21 on smart pointers