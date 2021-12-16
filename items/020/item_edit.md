1 // item status
# Inheritance

In C++, classes can form a hierarchy by *inheritance*. Let's look at a very simple example of a hierarchical coupling of three types:
```pmans
/* file: layout.cpp */
struct /*b*/ Base /*x*/ {
  int b;
  void member(){};
};
// sizeof(Base) -> sizeof(int)
struct /*b*/ Widget /*x*/ : /*b*/ Base /*x*/ {
  int w;
  void member(){};
};
// sizeof(Widget) -> 2*sizeof(int)
struct /*b*/ Other /*x*/ : /*b*/ Widget /*x*/ {
  int o;
  void member(){};
};
// sizeof(Other) -> 3*sizeof(int)

// memory layout for 'Other'
struct /*f*/ Other /*x*/ { // layout
 int b; // from Base
// int w; // from Widget
//  int o;
};
Other* optr = new Other{};
Base* bptr = optr;
Other* optr2 = bptr; // ok here (but not general) 

int main() {
  Other o{{{5}, 7}, 6};
  o.b = 5;
  o.w = 6;
  o.o = 7;
  o.Base::member(); // access 'shadowed' member using qualified type 
  o.Widget::member();
  o.member();
}
```
In this scenario the member function `member` has the same name in all three levels and is therefore shadowed; the different implementations can still be called using qualified names like `o.Base::...`

The hierarchy above introduces an *is-a* relationship between types in the *upwards* direction: 
- `Other` *is-a* `Widget` (and a `Base`)
- `Widget` *is-a* `Base`

This relationship is not necessarily true for the *downwards* direction:
- `Base` *might-be-a* `Widget` (and a `Other`) 
- `Widget` *might-be-a* `Other`

This relationship is what makes a hierarchy attractive: a derived type can be use as if it were a type more *up* in the hierarchy.

## Construction
In a hierarchy, sufficient constructors are required to be available for all participating classes.
E.g., if a default constructor is not available for a base class, a subclass cannot be default constructed:
```pmans
/* file: ctor_delete.cpp */
struct /*f*/ Base /*x*/ {
  int b;
  /*f*/ Base /*x*/() = delete;
};
struct /*b*/ Widget /*x*/ : /*f*/ Base /*x*/ {
  int w;
  /*b*/ Widget /*x*/() :  /*f*/ Base() /*x*/, w(...this from base...) {

  } // default construction
};
```
The construction mechanism constructs base classes before derived classes (in the order of appearance in the list of base classes) and before any other non-static members.

Constructors are not automatically inherited but can be explicitly inherited by using `using`:
```pmans
/* file: ctor_inherit.cpp */
struct /*f*/ Base /*x*/ {
  int b;
  /*f*/ Base /*x*/(int b) : b(b) {}
  /*f*/ Base /*x*/(int a, int b) : b(a + b) {}
  /*f*/ Base /*x*/(int a, int b, int c) : b(a + b * c) {}
};
struct /*b*/ Widget /*x*/ : /*f*/ Base /*x*/ {
  using /*f*/ Base /*x*/::/*f*/ Base /*x*/; // inherit all Base constructors
};

int main() {
  /*b*/ Widget /*x*/ w(5, 6);
  /*b*/ Widget /*x*/ w2(5, 6, 7);
}
```
> Why are base classes initialized before members?
<!-- 
> - more-derived types might already want to utilize the state of their "base-part" during initialization
-->

## Destruction
The destruction sequence starts with the execution of the destructor body of the derived class, followed by
- the destructors of non-static members (in reverse order of construction)
- the destructors of base classes (in reverse order of construction)

```pmans
/* file: dtor.cpp */
struct /*f*/ Base /*x*/ {
  int b;
  ~/*f*/ Base /*x*/() {}
};
struct /*b*/ Widget /*x*/ : /*f*/ Base1, Base2 /*x*/ {
  int w;
  ~/*b*/ Widget /*x*/() {/* ddd */}
};

int main() {
  /*b*/ Widget /*x*/ w{};
  // destruction sequence:
  // w.~Widget() // itself
  //   w.~w()    // members
  // w.Base2::~Base2() // base itself
  //   w.Base2::b()   // base members  
  // w.Base1::~Base1() // base itself
  //   w.Base1::b()   // base members
}
```
> Why is the order of destruction reversed?
<!--
> - more-derived type might depend on state of the base classes during destruction
-->

## Access control
For a `struct` the default mode for the access of inherited properties is `public` while the default mode when using the `class` keyword is `private`. 
The following illustrates the accessibility of inherited members depending on the specified access mode.
```pmans
/* file: access.cpp */
struct Base {
private: 
  int prv;   // access only from inside 'Base'
protected: 
  int pro; // + access from within more-derived classes
public:  
  int pub;   // + access from outside
};
```
**private**
```pmans
struct WidgetPrv : /*b*/ private /*x*/ Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // access is /*b7*/ private
    int pub = Base::pub; // access is /*b7*/ private   
};
```
- private access from within derived class 
- no additions to public interface of derived class

**protected**
```pmans
struct WidgetPro : /*b*/ protected /*x*/ Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // access is /*b9*/ protected
    int pub = Base::pub; // access is /*b9*/ protected    
};
```
- restrict access to inherited properties to classes in the hierarchy
- no additions to public interface

**private**
```pmans
struct WidgetPub : /*b*/ public /*x*/ Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // access is /*f9*/ protected
    int pub = Base::pub; // access is /*b6*/ public    
};
```
- fully extending the outward interface with the interface of the base (beside constructors)


```pmans
struct Base {};
struct Widget : public Base {
public: // implicit
    int m;
};
class Base {
public:
  int g;
};
class Widget : Base {
private: // implicit
    int m;
};
```

**granular access**

When inheriting, it is possible to expose parts of the inherited interface by using `using`. This allows to pick single properties of base classes and expose the using a different access mode: 
```pmans
struct WidgetPrv : /*b*/ private /*x*/ Base {
  /*b*/ protected /*x*/:    
    /*f*/ using /*x*/ Base::pro; // pro access is protected now 
    /*f*/ using /*x*/ Base::pub; // pub access is protected now 
    // /*f*/ using /*x*/ Base::prv; // still not allowed           
    ...
};
```
## Upcasting
Up to now we saw inheritance which accumulated all data members and non-static members in the more-derived classes. 
Looking at the very first snippet above, we could also have directly used a member of type `Base` and `Widget`, i.e., forming *has-a* relationships.
But only a hierarchy does provided *implicit upcasting*.
To see what this means and why can be attractive let's consider a function taking a `Base` parameter as reference:
```pmans
/* file: upcasting_flat.cpp */
struct /*f*/ Base /*x*/ {
  int b;
};
struct /*b*/ Widget /*x*/ : /*f*/ Base /*x*/ { // hierarchy: Widget "is-a" Base
  int w;
};
struct /*b*/ WidgetFlat /*x*/ { // flat: Widget "has-a" Base
  /*f*/ Base /*x*/ base;
  int w;
};

void func(/*f*/ Base /*x*/ &base) { base.b = 7; } 

int main() {
  /*b*/ Widget /*x*/ w{};
  func(/*f1*/ w);          // hierarchy: implicit upcasting
  /*b*/ WidgetFlat /*x*/ w2{};
  func(/*f7*/ w2.base);    // flat: "manual" member passing
}
```
Let's see what it looks like when a function returns an upcasted object:
```pmans
/*f*/ Base /*x*/ get_base() { return Widget{}; } // returning upcasted Base by-value

// usage
auto b = get_base(); 
```
> Is the returned value still a `Widget` somehow?
<!--
> - No, `b` is a `Base` (copy constructed from the `Base`-part of `Widget`)
-->

If it is desired to keep the `Widget`-part alive after returning, a pointer to a dynamically allocated `Widget` could be used:
```pmans
/*f*/ Base /*x*/ *get_base_ptr() { return /*b3*/ new Widget{}; } // returning upcasted Widget via pointer
// usage
auto* ptr = get_base_ptr();
delete ptr; // this calls ~Base(), is this enough, or a problem?
```
The returned pointer points to the `Base`-part of the layout of `Widget`; the `Widget`-part of the layout is still around. 
Let's assume a different `WidgetOwns` to emphasize this problematic situation:
```pmans
/* file: upcasting_leak.cpp */
struct /*b*/ WidgetOwns /*x*/ : /*f*/ Base /*x*/ {
  int *data;
  WidgetOwns() : data(/*b*/ new /*x*/ int) {}
  ~WidgetOwns() { /*b*/ delete /*x*/ data; };
};
```
`WidgetOwns` is now a resource owning class, which emphasizes the need for a proper deallocation at the end of its lifetime.

> Does the deallocation work as expected when deallocating using a `Base` pointer?
<!--
> - No, the hierarchy in use is not *polymorphic* : the deconstruction only invokes `~Base()` which is not aware of the need to call `~Widget()`. 
-->

**If** *polymorphic* behavior (via *virtual functions*) is expected from a hierarchy (which has consequences on the memory footprint and performance when invoking member function) this has to be explicitly demanded by the programmer.

## Virtual functions
Above we used a dynamically allocated object because it should outlive the scope of the returning function.
Without using *virtual functions*, the handle (to `Base`) is only handling the base class and does not know about the additional properties of the allocated `WidgetOwns`.
To guarantee a proper deallocation of `WidgetOwns`' resources (i.e., calling its destructor) we can define the destructor of `Base` as `virtual` and the destructor of `WidgetOwns` as `override`:
```pmans
struct Base {
  int b;
  /*b*/ virtual /*x*/ ~Base() = default;
};
struct WidgetOwns : Base {
  int *data;
  WidgetOwns() : data(new int) {}
  ~WidgetOwns() /*b*/ override /*x*/ { delete data; };
};
```
This has consequences: the hierarchy is now *polymorphic*. When a derived object is constructed, the final overriding resolution to any of the virtual functions is available at run time; this information is available at run time even if the object is casted between levels of the hierarchy.
Commonly this is implemented by adding an additional hidden pointer in the class layout of each polymorphic class (see next section).

When a `WidgetOwns` is constructed and upcasted to `Base` the *final override* destructor is the one from `WidgetOwns` which guarantees a complete deallocation the following snippet:
```pmans
Base *get_base_ptr() { return new WidgetOwns{}; } 
// usage
auto* b = get_base_ptr();
delete b; // final override is ~WidgetOwns();
```



Not only the destructor but any other non-static member function of can be defined as `virtual` and can be overridden in subclasses of the hierarchy: the most-derived (i.e., final) override is selected. 
Let's look at an example:
```pmans
/* file: vec_unique.cpp */
struct /*f*/ Base /*x*/ {
  virtual int calculate() { return 5; }
  virtual ~/*f*/ Base /*x*/() = default;
};
struct /*f*/ Widget1 /*x*/ : public /*f*/ Base /*x*/ {
  int calculate() override { return 1; }
  // ~Widget1() override {}; // not required -> implicit default dtor will override
};
struct /*f*/ Widget2 /*x*/ : public /*f*/ Base /*x*/ {
  int calculate() override { return 2; }
};

  // usage
  std::vector<std::unique_ptr</*f*/ Base /*x*/>> vec;
  vec.emplace_back(new /*f*/ Widget1 /*x*/{}); // implicit upcast
  vec.emplace_back(new /*f*/ Widget2 /*x*/{}); // implicit upcast
  vec.emplace_back(new /*f*/ Base /*x*/{});    // is already base

  for (auto &&item : vec) {
    std::cout << item->calculate() << std::endl; 
    // item->calculate() resolves to final overrides of 'calculate'
  }
```
This is an example of using a polymorphic hierarchy to be able to syntactically interact with each of them in the exact same way, i.e., calling the `calculate()` member.

> Why do we have to use dynamically allocated objects above?
<!--
> - different types with different object footprints cannot be stored in a container

> - native pointers (and also smart pointers) have a fixed size

> - pointers to *not* dynamically allocated objects cannot be used: 
objects need to outlive the scope of creation
-->
> What are the disadvantages?
<!--
> - one additional allocation per object (performance!)

> - for containers with contiguously allocated memory: objects are not stored contiguously, but the pointers (performance!)

> - semantics change to "reference semantics", e.g., a copy now copies the  (smart) pointer, not the pointed-to object. The default in C++ is "value semantics", e.g., it might be harder to reason about local code.
-->

### Virtual functions: (typical) implementation
Typically the lookup for overridden functions is implemented by introducing an extra pointer in each polymorphic object, which points to a *virtual function table* (there is exactly one for each polymorphic type). This table is a list of function pointers which were detected at compile time as the final overrides for each virtual function in the hierarchy.
```pmans
struct /*f*/ Base /*x*/ {
  void */*b*/ vptr /*x*/; // points to vtable for 'Base'
  virtual void test();
  virtual ~Base();
};
struct /*f*/ Widget /*x*/ : /*f*/ Base /*x*/ {
  void */*b*/ vptr /*x*/;  // points to vtable for 'Widget:Base'
  void test() override;
};
```
The call of a virtual function then uses the `vptr` to access the virtual function table and the calls the desired function by using the function pointer stored at a fixed offset in the table.

Note: Compared to the invocation of a non-virtual function this introduces a run time overhead. Also inlining is not possible for virtual function calls.

> Is the `override` keyword actually required? 
<!--
> - no, it is "just" to help to avoid mistakes, i.e., if a desired override does not match (due to a mistake in the signature) a virtual function

> - as the `override` keyword is optional, a virtual destructor does not have to be explicitly overridden: the implicit default destructor will override.
-->

## Abstract classes
In the above we defined members of the base class as virtual and overloaded them higher in the hierarchy.
It is also possible to define a virtual function as *pure virtual*: this demands an override somewhere higher in the hierarchy (which was optional so far).
Let's look at an example:
```pmans
/* file: abstract.cpp */
struct /*f*/ Base /*x*/ {
  /*b*/ virtual /*x*/ int calculate() /*b*/ = 0 /*x*/; // pure virtual
  virtual ~/*f*/ Base /*x*/() = default;
};
struct /*f*/ Widget /*x*/ : /*f*/ Base /*x*/ {
  int calculate() /*b*/ override /*x*/ { return 1; } // override required somewhere in the hierarchy
};

  // usage
  auto up = std::unique_ptr<Base>(new Widget{});   
```
Here, `Widget` is forced to implement the pure virtual member `calculate` in order to compile.
This can be used to enforce an interface by prescribing it via pure virtual function collected in an abstract base class.


## Downcasting
Above we have seen that upcasting in a hierarchy is implicitly available (due to the *is-a* relationship). 
Downcasting requires an explicit cast (no general *is-a* relationship can be assumed).
If a hierarchy is polymorphic (at least one virtual function is involved) `dynamic_cast` can be used to safely (i.e., run time type checking) downcast in a hierarchy:
```pmans
/* file: downcasting.cpp */
  /*f*/ Base /*x*/ *base = new /*f*/ Widget /*x*/{};              // upcasting
  auto *w = /*b*/ dynamic_cast /*x*/</*f*/ Widget /*x*/ *>(base); // downcasting (for polymorphic hierarchies)
  delete base;
```

<!--
  // background:
  // static_cast = check formal convertability of types at compile time (e.g.
  // base to derived) reinterpret_cast = compile time overrule: no checks at all

  // const_cast = remove constness of const aliasas which actually refer to
  // something non-const 
  // dynamic_cast = runtime resolution of implementation of polymorphic types (RTTI) virtual dtor = required only if deletion of derived via a base pointer is intended
  // reinterpret_cast = 
>

## Links
- Derived classes: https://en.cppreference.com/w/cpp/language/derived_class
- Virtual functions: https://en.cppreference.com/w/cpp/language/virtual
- Abstract class: https://en.cppreference.com/w/cpp/language/abstract_class
- Dynamic_cast: https://en.cppreference.com/w/cpp/language/dynamic_cast
- Sean Parent about polymorphism: https://www.youtube.com/watch?v=QGcVXgEVMJg