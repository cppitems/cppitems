9 // item status
# Inheritance

In C++, classes can form a hierarchy via *inheritance*. Let's look at a very simple example of a hierarchy of types:
```pmans
/* file: layout.cpp */
struct /*b*/ Base /*x*/ {
  int b;
  void member(){};
};
struct /*b*/ Widget /*x*/ : /*b*/ Base /*x*/ {
  int w;
  void member(){};
};
struct /*b*/ Other /*x*/ : /*b*/ Widget /*x*/ {
  int o;
  void member(){};
};
// footprint of layout
struct /*f*/ OtherFootprint /*x*/ {
  int b; // from Base
  int w; // from Widget
  int o;
};

int main() {
  Other o{{{5}, 7}, 6};
  o.Base::b = 5;
  o.Widget::w = 6;
  o.Other::o = 7;
  o.Other::member();
  o.Base::member();
  o.Widget::member();
}
```
In this scenario the member function `member` has the same name at all three levels and is therefore shadowed; the different implementations can still be called using qualified names like `o.Base::...`

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
  /*b*/ Widget /*x*/() : /*f*/ Base /*x*/(), w() {} // implicit construction mechanism
};
```
The construction mechanism constructs base classes before derived classes in the order of appearance in the list of base classes and before any non-static members.

Constructors do not participate in the inheritance automatically but can be explicitly inherited by using `using`:
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

## Destruction
The destruction sequence starts with the execution of the destructor body of the most derived type followed by
- the destructors of non-static members (in reverse order of construction)
- the destructors of base classes (in reverse order of construction)

```pmans
/* file: dtor.cpp */
struct /*f*/ Base /*x*/ {
  int b;
  ~/*f*/ Base /*x*/() {}
};
struct /*b*/ Widget /*x*/ : /*f*/ Base /*x*/ {
  int w;
  ~/*b*/ Widget /*x*/() {}
};

int main() {
  /*b*/ Widget /*x*/ w{{5}, 6};
  // destruction sequence:
  // w.~Widget()
  // w.~w 
  // w.Base::~Base()
  // w.Base::b
}
```
> Why is the order of destruction reversed?

## Access control
For a `struct` the default mode of inheritance is `public` while the default mode when using the  `class` keyword is `private`. 
The following illustrates the accessibility of members of base depending on the "inheritance mode"
```pmans
/* file: access.cpp */
struct Base {
  private: int prv;
  protected: int pro;
  public:  int pub;
};

struct WidgetPrv : /*b*/ private /*x*/  Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // pro access is private
    int pub = Base::pub; // pub access is private   
};
struct WidgetPro : /*b*/ protected /*x*/ Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // pro access is protected
    int pub = Base::pub; // pub access is protected    
};
struct WidgetPub : /*b*/ public /*x*/ Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // pro access is protected
    int pub = Base::pub; // pub access is public    
};
```
When inheriting privately, it is possible (similar to the constructors above) to expose parts of the inherited interface by using `using`:
```pmans
struct WidgetPrv : /*b*/ private /*x*/ Base {
  /*b*/ public /*x*/:    
    /*f*/ using /*x*/ Base::pub; // pub access is public now 
    /*f*/ using /*x*/ Base::pro; // pro access is public now    
};
```
## Upcasting
Up to now we saw inheritance which accumulated all data members and non-static members in the most-derived class. 
Looking at the very first snippet above, we could also directly used a member of type `Base` or `Widget` instead of inheriting from these classes as we did not make use of the hierarchy in any way.
But only a hierarchy does provided *implicit upcasting*.
To see what this means and why can be attractive let's consider a function taking a `Base` parameter as reference:
```pmans
/* file: upcasting_flat.cpp */
struct /*f*/ Base /*x*/ {
  int b;
};
struct /*b*/ Widget /*x*/ : /*f*/ Base /*x*/ { // hierarchy
  int w;
};
struct /*b*/ WidgetFlat /*x*/ { // flat
  /*f*/ Base /*x*/ base;
  int w;
};

void func(/*f*/ Base /*x*/ &base) { base.b = 7; }

int main() {
  /*b*/ Widget /*x*/ w{};
  func(w); // hierarchy: implicit upcasting
  /*b*/ WidgetFlat /*x*/ w2{};
  func(w2.base); // flat: "manual" member passing
}
```
Let's see what it look like when a function returns an upcasted object:
```pmans
/*f*/ Base /*x*/ get_base() { return Widget{}; } // returning upcasted Base by value
// usage
auto b = get_base();
```
> Is the returned value still a `Widget` somehow?
> If not, how could `get_base` be adopted to accomplish this?

```pmans
/*f*/ Base /*x*/ *get_base_ptr() { return new Widget{}; } // returning upcasted Widget via pointer
// usage
auto* ptr = get_base_ptr();
delete ptr; // this calls ~Base(), is this enough?
```
Let's assume a different `WidgetOwns` to emphasize the problem:
```pmans
/* file: upcasting_leak.cpp */
struct /*b*/ WidgetOwns /*x*/ : /*f*/ Base /*x*/ {
  int *data;
  WidgetOwns() : data(/*b*/ new /*x*/ int) {}
  ~WidgetOwns() { /*b*/ delete /*x*/ data; };
};
```
> Does the deallocation work as expected when deallocating using a `Base` pointer?


## Dynamic dispatching
Above we used a dynamically allocated object because it should outlive the scope of the returning function.
Without using *virtual functions*, the handle (to `Base`) is only handling the base class and does not know about the additional properties of the allocated `WidgetOwns`.
To guarantee a proper deallocation of the `WidgetOwns` resources (i.e., calling its destructor) we can define the destructor of `Base` as `virtual` and the destructor of `WidgetOwns` as `override`:
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
This has consequences: the hierarchy is now *polymorphic*. When a most-derived object is constructed, the final overriding resolution to any of the virtual functions is available at run time; this information is available at run time (vptr, see below) even if the object is casted between members of the hierarchy:
When a `WidgetOwns` is constructed and upcasted to `Base` the final overriding destructor is the one from `WidgetOwns` which guarantees a complete deallocation the following snippet:
```pmans
Base *get_base_ptr() { return new WidgetOwns{}; } 
// usage
auto* b = get_base_ptr();
delete b; // final override is ~WidgetOwns();
```

Not only the destructor but any other non-static member function of can be defined as `virtual` and can overridden in sub-classes of the hierarchy: the most-derived override is then selected. 
Let's look at an example:
```pmans
/* file: vec_unique.cpp */
struct /*f*/ Base /*x*/ {
  virtual int calculate() { return 5; }
  virtual ~/*f*/ Base /*x*/() = default;
};
struct /*f*/ Widget1 /*x*/ : public /*f*/ Base /*x*/ {
  int calculate() override { return 1; }
};
struct /*f*/ Widget2 /*x*/ : public /*f*/ Base /*x*/ {
  int calculate() override { return 2; }
};

  // usage
  std::vector<std::unique_ptr</*f*/ Base /*x*/>> vec;
  vec.emplace_back(new /*f*/ Widget1 /*x*/{});
  vec.emplace_back(new /*f*/ Widget2 /*x*/{});
  vec.emplace_back(new /*f*/ Base /*x*/{});

  for (auto &&item : vec) {
    std::cout << item->calculate() << std::endl;
  }
```
This is an example of using a polymorphic hierarchy to treat a group of types in the same way (here: calling `calculate()`).

> Why do we have to use dynamically allocated objects here?

> What are the disadvantages?

### Dynamic dispatch implementation
Typically the lookup for overridden functions is implemented by introducing an extra pointer in each polymorphic object, which points to a *virtual function table* (there is exactly one for each polymorphic type). This table is a list of function pointers which were detected at compile time as the final overrides for each virtual function were determined.
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
The call of a virtual function then uses the `vptr` to access the function table and the calls the desired function (at a fixed offset) by following the pointer stored in the table. Function inlining is not possible for virtual function calls.

> Is the `override` keyword actually required?

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
  int calculate() /*b*/ override /*x*/ { return 1; } // override required somewhere in hierarchy
};

  // usage
  auto up = std::unique_ptr<Base>(new Widget{});   
```
Here, `Widget` is forced to implement the pure virtual member `calculate` in order to compile.
This can be used to enforce an interface by prescribing it via pure virtual function collected in an abstract base class.


## Downcasting
Above we have seen that upcasting in a hierarchy is implicit (upwards we have an *is-a* relationship). 
Downcasting requires an explicit cast (not an *is-a* relationship downwards).
If a hierarchy is polymorphic (at least one virtual function is involved) `dynamic_cast` can be used to safely downcast in the hierarchy:
```pmans
/* file: downcasting.cpp */
  /*f*/ Base /*x*/ *base = new /*f*/ Widget /*x*/{};              // upcasting
  auto *w = dynamic_cast</*f*/ Widget /*x*/ *>(base); // downcasting (for polymorphic hierarchies)
  delete base;
```

## Links
- Derived classes: https://en.cppreference.com/w/cpp/language/derived_class
- Virtual functions: https://en.cppreference.com/w/cpp/language/virtual
- Abstract class: https://en.cppreference.com/w/cpp/language/abstract_class
- Dynamic_cast: https://en.cppreference.com/w/cpp/language/dynamic_cast
- Sean Parent about polymorphism: https://www.youtube.com/watch?v=QGcVXgEVMJg