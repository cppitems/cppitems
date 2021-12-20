1 // item status
# Smart Pointers

Raw/native pointers are a versatile tool in C. 
They are used for
- (a) cheaply passing an object for inspection purposes (no modification) 
<!--  // void func(/*f*/ const Widget & /*x*/w); // c++ -->
```pmans
/* file: C */
void func(/*b*/ const Widget * /*x*/w) { // C: pointer is the only option for "an alias"
    int m = w->m; // ok read
    w->m = 5;     // nok write
    // can inspect Widget, but cannot modify
    // no pointer arithmetic intended (as it is unclear what the size of an array would be)
}
``` 
```pmans
/* file: C++ */
void func(/*b*/ const Widget & /*x*/w) { 
    // (1) ref never "NULL" (calling context has to ensure that it is not (and does not become) a "dangling" reference)
    // (2) ref does not need dereferencing
    int m = w.m; // ok read
    w.m = 5;     // nok write
    // no pointer arithmetic accessible
}
``` 
- (b) cheaply passing an object intended to be modified by the callee
```pmans
/* file: C */
void func(/*b*/ const Widget * /*x*/array, /*b*/ int /*x*/ size) {
    // pointer arithmetic to access array contents
}
``` 
```pmans
/* file: also C */
struct Vec {
    Widget *data;
    int size;
}; // Note: not dtor in C
void func(/*b*/ const Vec * /*x*/array){
    // pointer arithmetic and const access array contents 
}
```   
```pmans
/* file: C++ */
void func(/*b*/ const std::array<Widget,N>& /*x*/ array); // const ref to std::array (fixed size)
void func(/*b*/ const std::vector<Widget>& /*x*/ array); // const ref to std::vector (dynamic size)

``` 
- (d) passing handles to objects created with dynamic storage duration (or other resources with similar needs)
<!-- // c++ solution here? -> smart pointer -->
```pmans
/* file: C */
void func(Widget *w); // function accepting a pointer to widget
/*b*/ Widget * /*x*/ptr = /*f*/ new /*x*/ Widget(); // object creation, dynamic lifetime
func(ptr);  // pass into function: should the function call delete on ptr?
/*f*/ delete /*x*/ ptr; // manual termination required
``` 
```pmans
/* file: C++ */
void func(Widget &w); // function accepting a reference to a widget
/*b*/ auto /*x*/ ptr = std::unique_ptr<Widget>(/*f*/ new /*x*/ Widget()); // object creation, dynamic lifetime, single owner
func(*ptr);  // function will pass an alias to the wrapped object
// manual termination not required
``` 

- (e) returning a dynamic resource from a function
```pmans
/* file: C */
/*b*/ Widget * /*x*/get_widget(){ 
    return /*f*/ new /*x*/ Widget(); // return from a function
}
```
```pmans
/* file: C++ */
/*b*/ Widget * /*x*/get_widget(){ // use a smart pointer -> see below example
    return /*f*/ new /*x*/ Widget(); // return from a function
}
```

> What could be used in C++ for the situations above?
> - (a)-(c) see `C++` versions in snippets above 
> - (d)-(e) a smart pointer, e.g., `std::unique_ptr` or `std::shared_ptr`, is the go-to solution to manage  dynamically allocated resources in c++

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
  /*b2*/ ++ptr;                         // (6) increment // what does this? sizeof(Widget) -> step that far ahead in address space
  /*b2*/ --ptr;                         // (7) decrement // other direction 
  if (ptr /*b2*/ != &w) {throw;};       // (8) operator!= 
  ptr /*b1*/ = nullptr;                 // (9) nullable
  if (ptr /*b2*/ == &w) {throw;};       // (10) operator== // when is this true? if same address (only compare equal type pointers)
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
std::unique_ptr<Widget> get_widget2() {
  // (1) passing pointer directly (in return expression)
  return /*b*/ std::unique_ptr /*x*/<Widget>(new Widget{});
  // (2) ... via make_unique (in return expression)
  return /*b*/ std::make_unique /*x*/<Widget>(); 
  // (3) create object, rely on implicit convertions (due to return type of function)
  auto tmp = new Widget{};
  return w; // implicit conversion to unique_ptr
  // (4) create directly and return
  std::unique_ptr<Widget> up = new Widget{}; // currently myfunc owns it
  return up; // we do not need to std::move() explicitly (copy elision/RVO is guaranteed for return statements)  
}
// usage example
auto ptr = myfunc(); // ptr is now owned by caller;
// what do we know:
// (1) we obtained a dynamic resource from the function that we own now
// (2) what CAN we do with it? move, but not copy
// (3) what do we HAVE to do with it? nothing, unique_ptr does that for us, whatever it is

```

> Why do we even use pointers in C++ and not only references?
> - reference cannot be reassigned 
>   - it might look like an assignemnt, but it is copy-assigning the object: 
>      ```pmans
>      refA = refB; // copy-assign the objects
>      ```
>   - pointers can be reassigned: 
>      ```pmans
>      ptrA = ptrB; // overwrite memory location
>      ```
> - if you construct a reference to a dynamic resource, there is no way to check if it is still alive, provider of reference has to ensure it is not (and does not become) *dangling*:
>      ```pmans
>      T& ref = *dynamic_resource; // cannot check a != nullptr`
>      if (&ref == nullptr) // not available for reference
>      T* ptr = dynamic_resource; // cannot check a != nullptr`
>      if (ptr == nullptr) // ok for pointer
>      ```
> - a reference cannot transfer "ownership" of a dynamic resource (there is no way to `delete` via a reference)
> - BUT: if non of the above constraints is relevant: use a reference instead of a pointer!

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
>std::unique_ptr<Widget> w3 = get_widget2(); // ctor, ok
>std::unique_ptr<Widget> w2 = get_widget2(); // ctor, ok
>w2 = w3;                         // (1) copy assign, -> not available (if it would be: two unique ptrs to one resource)
>std::unique_ptr<Widget> w4 = w2; // (2) copy ctor, -> same here
>std::unique_ptr<Widget> w3 = std::move(w2); // (3) move ctor, yes allowed (e.g. to move ownership "out of a function scope")
>w2 = std::move(w3));                        // (4) move assign, what about w2's original resource? -> is released
>```



> How is the automatic release of the resource implemented?
> - in the destructor of `unique_ptr` **OR** at a move assignment

> What about pointer arithmetic and other operators? Are they supported?
> - arithmetic, e.g., `operator++`, `operator--` ?? (1) not allowed (but of course you can do that: `.get()` the raw pointer)
> - comparison, e.g., `opterator==`, `opterator!=` ?? (2) yes, we definitely want a comparison to `nullptr`

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
  return std::unique_ptr<FILE, decltype(/*b7*/ deleter)>(/*f*/ std::fopen /*x*/(name, mode), /*b7*/ deleter);
};

int main() {
  auto filename = "data.json";
  auto mode = "r";
  auto file = open(filename, mode);
  if (file) {
    parse(file); // (1) does this work like this, is there an implicit conversion from a unique_ptr to raw ptr?
    // no:  there is no implicit conversion, because this could easily lead to semantic embeddings (e.g. construction) violating the idea of "uniqueness"
    parse(file.get()); // (2) this is required
  }
}
```
> Is there an implicit conversion from `std::unique_ptr` to the underlying raw pointer (see (1) in the snippet above) ?
> - no, one as to rely on `.get()` or operators:
>```pmans
>    parse(file.get()); // use functionality of unque_ptr
>    parse(&*file); // more cumbersome (this is why .get() is there)
>```
This demonstrates that a `std::unique_ptr` can also be used to manage resources which are not (directly) created with `new` and released with `delete`.

#### Managing an array
When a `std::unique_ptr` is used to manage an array, indexed access is provided via the `[]` operator:
```pmans
std::unique_ptr<double/*b2*/ []> up(new double/*b3*/ [10]);
up/*b1*/ [9/*b1*/ ] = ...; // set last element of array
```
The relase of the array (which now requires `delete[]`) is realized by an appropriate deleter in the template specialization of `std::unique_ptr`.

> `std::nique_ptr` summary, what do we remember?
> - indicates ownership
> - try to make pointers safer, but keep 'power'
>   - safe: deleters are automatically called at end of lifetime
>   - power: interface/semantics tries to imitate raw-pointer semantics whenever possible
>   - reassiging: releases old resource (including deletion)
> - cannot be copied
> - only one handle per resource at a time (unique) is allowed
> - lightweight (no footprint overhead)
> - custom deleter (might introduce some footprint overhead); defaults are `delete`, `delete[]`



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
// std::unqiue_ptr<Widget> up1 = std::make_unqiue<Widget>(); // using make_shared
std::shared_ptr<Widget> sp2 = std::shared_ptr<Widget>(/*b3*/ new Widget{}) // ctor + new
std::unique_ptr<Widget> up1 = get_widget2(); // unique_ptr
std::shared_ptr<Widget> sp3 = std::move(up1); // from rvalue unique_ptr
std::shared_ptr<Widget> sp4 = get_widget2(); // implicit from rvalue unique_ptr

// state of up1 here? 
// what is the reference count of sp1, sp2, sp3, and sp4 here? 
```
> Is it possible to convert between `std::unique_ptr` and `std::shared_ptr`?
> - yes, an implicit conversion is available from `unique_ptr` to `shared_ptr` (from rvalues only)
> - a conversion from `shared_ptr` to `unique_ptr` is not allowed (would need to invalidate all references)


> Which restrictions (SMFs) can we expect to be lifted for `std::shared_ptr` (compared to `std::unique_ptr`)?
> - copy assignment and copy construction is expected to be available


> Does the principle of the automatic release mechanism differ from `std::unique_ptr`?
> - identical in general; but the condition for its execution differs: only a decrement of the reference count to '0' triggers a release of the resource;
> - this might also happen during a copy assignment (the refcount for the assigned resource is increased), not only when a variable goes out of scope

```pmans
auto sp1 = (r1); // (1) 1 sp object, count_r1 = 1
auto sp2 = (r2); // (2) second sp object, count_r2 = 1
auto sp3 = sp1; // (2) third sp object, count_r1 = 2
// two handles to r1, one handle to r2
sp2 = sp1; 
// 1. sp2 was a handle to r2
// 2. as it should is assigned to be a handle to r1, refcount for r2 should be decremented -> count_r2 = 0
// -> is there a resource released already now? -> yes r2 is release (as refcount dropped to 0)
// 3. we now have a third handle to r1: sp2 -> refcount increase for r1 -> count_r1 = 3
```

> What about pointer arithmetic and other operators?
> - same as for `std::unique_ptr`


### Overhead: what does a `std::shared_ptr` look like
If we decide to use a `std::shared_ptr` instead of a `std::unique_ptr` or a raw pointer, what can we expect in terms of performance? To argue about this, let's look again at what (a small part of) a simplified implementation looks like:
```pmans
template <class T> class /*f*/ shared_ptr /*x*/ {
  struct /*f*/ ControlBlock /*x*/ {
    int count = 1; 
    int weak_count = 0;
    // some more members in stdlib
  };
  /*b*/ ControlBlock *cb /*x*/; // (1) member A -> for refcounting
  /*b*/ T *ptr /*x*/; // (2) member B -> same as for unqiue_ptr

  void /*f*/ increment /*x*/() {
    if (cb)
      ++cb->count;
  }
  void /*f*/ decrement /*x*/() {
    if (cb && --cb->count == 0) {
      delete ptr; // wrapped resource
      delete cb; // overhead
    }
  }

public:
  // usage:
  // auto sp = shared_ptr<Widget>(/*b3*/ new Widget{});
  /*f*/ shared_ptr /*x*/(T *ptr) : cb(/*b3*/ new ControlBlock()), ptr(ptr) {}
  // usage:
  // auto other = shared_ptr<Widget>(new Widget{});
  // auto sp = other;  
  /*f*/ shared_ptr /*x*/(const /*f*/ shared_ptr /*x*/ &other) : cb(other.cb), ptr(other.ptr) {
    // (1) TODO: copy ctor
    // incremenet ref count (of the shared resource)
  }
  /*f*/ shared_ptr /*x*/(/*f*/ shared_ptr /*x*/ &&other) : cb(other.cb), ptr(other.ptr) {
    // (2) TODO: move construction
    // -> moved-from shared_pointer should not participate anymore in managing the resource
    // -> no increment/decrement
  }
  /*f*/ shared_ptr /*x*/ &operator=(const /*f*/ shared_ptr /*x*/ &other) {
    // (3) TODO: copy assign
    // -> decrement count of current resource (if a decrement counts to  '0' -> release)
    // -> increment count of newly-assigned resource
    return *this;
  }
  /*f*/ shared_ptr /*x*/ &operator=(/*f*/ shared_ptr /*x*/ &&other) {
    // (4) TODO: move assign
    // -> decrement count of current resource
  }
  ~/*f*/ shared_ptr /*x*/() { 
    // (5) TODO:
    // decrement -> (if a decrement counts to  '0' -> release)
  }
  ...
  T *operator->() const { return ptr; }
  T &operator*() const { return *ptr; }
};
template <typename T, typename... ARGS>
/*f*/ shared_ptr /*x*/<T> make_shared(ARGS&&... args) {
  // do sth. here: -> use a single allocation for cb and ptr  
  return /*f*/ shared_ptr /*x*/<T>(new T(std::forward<ARGS>(args)...));
}
// why can make_shared be an advantage compared to the 'regular' ctors?

```
It is visible that the construction (of the "original" handle ) triggers a subsequent dynamic allocation for the `ControlBlock`. The obtained pointer is stored as member `cb` in the `shared_pointer` additionally to the pointer of the managed object `ptr`. It is apparent that many of the SMFs will need to access the `ControlBlock`.
This means a `shared_ptr` does introduce some overhead (memory and access). This has to be considered for practical applications: if very small objects are managed by a `shared_ptr`, the relative increase of the memory footprint is not negligible. It might still be practical to use a `shared_ptr` for small objects, depending on the application context.

> What is expected to happen in the body of the three SMFs marked above (to implement the reference counting)?
> - see comments above


> Why is the `ControlBlock` a structure and not simply a integral type?
> - in this simple case, could be just a pointer to an (atomic) integer; in the stdlib, some more members are present in the `ControlBlock` structure to support advanced features of a `shared_ptr`: weak reference counting, and aliasing construction (access with offset)

> Would a static member be sufficient for reference counting, too?
> - no, a static member could only count references for all handles to shared pointers per type, but not per resource.


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
  auto /*f2*/ sp = get_widget(); // (1) one sp, count=1
  std::cout << /*f2*/ sp.use_count() << std::endl; //  1
  auto /*f3*/ sp2 = /*f2*/ sp; // copy ctor
  std::cout << /*f2*/ sp.use_count() << std::endl; //  2
  std::cout << /*f3*/ sp2.use_count() << std::endl; // 2
  std::cout << (/*f3*/ sp2 == /*f2*/ sp) << std::endl; // expect to compare equal the resource that is managed
  {
      auto /*f3*/ sp3 = /*f2*/ sp; // copy ctor 
      std::cout << /*f2*/ sp.use_count() << std::endl; // 3
  }
  std::cout << /*f2*/ sp.use_count() << std::endl; //  2
```

> What are the reference counts in the above snippet?
> - see inline comments


**Capturing the managed object outside**
```pmans
  Widget *ptr = nullptr;
  {
    auto sp = get_widget(); // (1a) obtain sp to a resource
    ptr = sp.get(); // (2) extract raw pointer (which is also a handle to the resource)
  } // (1b)
  ptr->m = 5; // (3) is this ok?
  // nok: ptr points to a memory location which is already deleted (due to the sp end of lifetime)
```
> Is it safe to perform the last line of the snippet above?
> - no, the resource `ptr` is pointing to is deallocated at (1b)



**Managing the same resource more than once**
```pmans
  void func(shared_ptr<Widget>& sp);
  void func(Widget&);
  void func(Widget*);
  auto sp1 = get_widget();
  auto sp2 = get_widget();
  std::shared_ptr<Widget> sp3(sp1); // (1) copy ctor
  std::shared_ptr<Widget> sp4(sp2.get()); // (2) ctor from pointer
  // pass resource to a function call
  func(...);
  // how many control blocks do exist now? -> TWO control block for ONE resource!
```
> What does it mean if a resource is managed "more than once"?
> - this means that two independent reference counting control blocks exist for a single shared resource; this situation should never be created

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
> - returning a `shared_ptr` constructed from `this` leads again to a "double management" (like above)
> - to support such a situation, the object has to know that it is managed by a shared pointer and somehow needs access to the respective control block (see next example).

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
> -  "thread-safe" referes to increments and decrements (meaning SMF-calls) are thread-safe
> - you can safely use the SMFs from different threads without further synchronization
> - increments and decrements are typically synchronized using atomic operations
> - about access to the managed resource: the share_pointer is **NOT** concerned what the managed resource provides in terms of having a thread-safe interface!
<!--
> - the modifying access to the shared_ptr (i.e., its control block) is thread-safe
> - the thread-safety-ness w.r.t. to the access of the managed resource is not influenced by the shared pointer: the shared pointer does not synchronize modifying access to the resource in any way; the access is direct.
-->

## Side Discussion
### Question: how to express that a function argument points to (or references) a valid (i.e non null) resource (which is managed by a smart pointer on the caller's site) to be accessed, but the function will not pariticipate in ownership/lifecycle mgnt?
```pmans
void func(?TYPE? widget); // which type to choose?
// usage
shared_ptr<Widget> sp = ...;
func(?HOW_TO_BIND?);
```
- Option 1: construct a new smart pointer with an empty deleter? -> expressiveness of shares_ptr is lost -> not good
```pmans
void func(shared_ptr<Widget> widget);
// usage
shared_ptr<Widget> sp = ...;
auto deleter = [](Widget *){};
func(shared_ptr<Widget,decltype(deleter)>(widget.get(),deleter))
```
- Option 2: use raw pointer? why not, altough: function implementer might be tempted to check for `nullptr` (at least syntactically there is not hint that nullptr is excluded)
```pmans
void func(Widget* widget);
// usage
shared_ptr<Widget> sp = ...;
func(widget.get());
```
- Option 3: use a reference? why not, altough: caller must ensure that reference is never "dangling" (but is is anyway always the underlying rule: references have to be valid)
```pmans
void func(Widget& widget);
// usage
shared_ptr<Widget> sp = ...;
func(*widget);
```
- Option 4: use std::weak_ptr? no advantage over a shared_ptr (beside opting in for ownership conditionally is required)




## Links

- `std::unique_ptr`: https://en.cppreference.com/w/cpp/memory/unique_ptr
- `std::shared_ptr`: https://en.cppreference.com/w/cpp/memory/shared_ptr
- `std::weak_ptr`: https://en.cppreference.com/w/cpp/memory/weak_ptr'
- `std::enable_shared_from_this`: https://en.cppreference.com/w/cpp/memory/enable_shared_from_this
- CPPCON2019 talk on smart pointers: https://www.youtube.com/watch?v=xGDLkt-jBJ4
- Blog entry on `std::make_unique`: https://abseil.io/tips/126
- Scott Meyers, Effective Modern C++: Items 18-21 on smart pointers