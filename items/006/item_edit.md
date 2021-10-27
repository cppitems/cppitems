1 // item status
# Overview: value categories and references

## Motivation

Additionally to a *type*, each expression is also characterized by its *value category*.
This is true for C and C++.
In C, there are no references (but pointers) and each expression  either belongs to
- the *lvalue expression* category (designating objects with a storage location) or to
- the *rvalue expression* category (no storage location associated).

The consequence is that *rvalue expressions* (rvalues) cannot stand on the left-hand-side of an assignment, as no storage location is associated, which could serve as target for a meaningful assignment. So rvalues can solely appear on the right-and-side of an assignment, hence their name.
On the other hand *lvalue expressions* (lvalues) can be the target for an assignment, i.e., can stand on the left-and-side but can also be used on the right-hand-side of an assignment.

Some examples in C:
```pmans
/* compile: clang -std=c11 */
typedef struct widget { int i; } Widget;
Widget return_tmp() { return (Widget){1};}
Widget *return_ptr() {return &gw;};
Widget gw = {2}; // global widget 
  ...
  Widget w = {3};            // (0) init, from list
  Widget w2 = return_tmp();  // (1) init, from rvalue 
  // how does a modern compiler react to (1)
  // 1. If all definitions are available in this compilation unit: constructs w2 "in-place" (optimization)
  // 2. why is this possible: because the compiler can analyse the function 'return_tmp' and it is quite simple
  return_tmp() = w; // does not work, (non-locatable) rvalue on lhs
  w.i = 5;                   // (2) 'w.i' is lvalue, '5' is rvalue
  // human compiler:
  // 1. "5" is fixed at compile time
  // 2. could be inlined/optimized during compilation
  // 3. does not "need" a location -> also not providing one
  ++5; // does not work (would need to write to a non-modifyable location)
  // question: does this work?
  class 5 {} // no: class names must start with _ or letter from a-zA-Z
  class _5 {} // yes (but not in C and not "inline")
  5 = w.i;                   // (3) ERROR, '5' is rvalue
  return_tmp() = (Widget){5};// (4) ERROR, 'return_tmp()' is rvalue
  return_ptr()->i = 5;       // (5) 'return_ptr()->i' is lvalue
  gw.i = 5; // equivalent effect of (5)
```
Let's look at a similar same code snippet again in C++ when also using references:
```pmans
/* compile: clang++ -std=c++17 */
struct Widget { int i; int j };
Widget gw = {1,2}; // global widget
Widget return_tmp() { return Widget{2}; }
Widget &return_ref() { return gw; } // this is now different to C above: we return an alias to the global Widget gw
void take_rref(Widget &&rref) {}
  ...
  Widget w{3};              // (0) init, from list
  Widget w2 = return_tmp(); // (1) init, from prvalue 'return_tmp()'
  w.i = 5;                  // (2) 'w.i' is lvalue, '5' is prvalue
  return_tmp() = Widget{4}; // (3) 'return_tmp()' is prvalue (temporary)
  int i = return_tmp().i;   // (4) 'return_tmp().i' is a xvalue (temp. materialization)
  return_ref().i = 5;       // (5) 'return_ref().i' is lvalue (simply aliasing access to 'gw')
  return_ref() = Widget{5}; // (6) 'return_ref()' is lvalue (simply aliasing access to 'gw')
  take_rref(std::move(w));  // (7) 'std::move(w)' is xvalue

  // on refences:
  // references cannot be "changed":
  Widget &wref = gw; // ok, init reference as alias for gw
  wref = gw2; // this is valid?
  // human compiler:
  // 1. compilers sees this:
  gw = gw2; 
  // 2. -> copy-assign (not "resetting")
  
  // "resetting" the reference to alias another object is not possible
  &wref = "other address" // does not "reseed", but copy-assign a memory address

```
Closely looking at the line comments above we can see that there are three value categories mentioned:  
- *lvalue*
- *prvalue*
- *xvalue*

And additionally there are *lvalue references* and *rvalue references* in use.
References can bind to expressions providing an alias to the object determined by the evaluation of the expression:
- rvalue references (`Widget &&`) can bind to xvalue and prvalue expression, i.e., expressions determining a movable object).
- lvalue references (`Widget &`) can bind to lvalue expressions, i.e., expression which determine a non-temporary object which is non-movable.

An overview is provided in the image below: 

![value_categories](https://raw.githubusercontent.com/cppitems/cppitems/master/items/006/value_categories.svg)
```pmans
// prvalue "pure rvalue"
int i = 5; // '5' is a prvalue
Widget w = Widget{5}; // 'Widget{5}' is a prvalue
// lvalue: any variable with a name !!!
int i = 5; // 'i' is an lvalue
Widget w = Widget{5}; // 'w' is a lvalue
Widget&& rref = Widget{5}; // 'rref' is a lvalue
// xvalue "expiring value"
int j = std::move(i); // 'std::move(i)' is xvalue
Widget&& rref = Widget{5}; // 'rref' is not an xalue !!! (but an lvalue of type rvalue reference)
take_rref(std::move(rref)); // 'std::move(rref)' is xvalue
```


## Value categories of expressions

Let's solely consider value categories w.r.t. a `Widget` type  with a single `int` member
```pmans
struct /*f6*/ Widget {
  int /*f1*/ m;
};
```
**lvalue** expressions determine a *locatable* *non-movable* object of the type `Widget`. 
All lvalue expressions determining a `Widget` object are marked in the snippet below:
```pmans
  Widget var;
  /*b3*/ var.m = 5;                      // (1)
  Widget /*f5*/ &lref = /*b3*/ var;             // (2)
  Widget *ptr = &/*b4*/ lref;            // (3)
  /*b4*/ *ptr = /*b3*/ var;                     // (4)
  const Widget /*f6*/ &clref = Widget(); // (5) // clref is an alias to a temporary
                                  // (which is kept alive as long as the reference is value)
  int i = /*b5*/ clref.m;                // (6)
 ```
All variable names are lvalue expressions, e.g., `var` in (2) or `lref` in (3) or  `clref` in (5).
The pointer `ptr` in (3) itself is not determining an object of type `Widget`;
but a dereferenced pointer `*ptr` (4) does.
A temporary object can bind to a lvalue reference `clref` (5) if the reference aliases access to a `const Widget`;
this binding extends the lifetime by means of the lifetime of the reference variable `clref` (6).

> Are references always 'const'?
 > - the object a reference aliases cannot be changed (and has to be assigned at initialization) 
>```pmans
>Widget w{};
>Widget &ref = w; // from this line on, ref aliases access to w
>Widget other{};
>ref = other; // (1) this line does not "reassign" w.r.t. the reference; it is equivalent to the next line
>w = other; // (2) equivalent assignment (i.e., assignment operator)
>```
 > - when saying "const reference" this is typically a short form for "reference providing `const` access to an object", e.g., (1) is ment and not (2) 
>```pmans
>Widget w{};
>const Widget& cref = w; // (1) cref provides an alias for const access to w
>Widget &const ref = w; // (2) this would be a const reference, providing non-const access to w
>```
> - a consequence of the above is that references cannot be set to "null" 
>```pmans
>Widget w{};
>Widget &ref = w;
>ref /*b1*/ = nullptr; // ERROR
>```

<!--
> - ... but they can be "dangling" if the alias object is not available >anymore
>```pmans
>int& return_dangling_ref() {
>    Widget w{};
>    return w,i;
>}
> ...
> int& /*b*/ dangling /*x*/ = return_dangling_ref();
>```
-->

> Informally, how would you characterize lvalues?
 > - as expressions which provide a handle to an object stored somewhere in memory 
 > - the object determined is not a temporary object but an object with clear expectations on its lifetime and state.


**prvalue** expressions can initialize a `Widget` object but are not (yet) affiliated with locatable `Widget` object.  Depending on the context, a prvalue can be a temporary object or directly initialize a variable. In the following all prvalue expressions for `Widget` are marked:
```pmans
  const Widget &clref = /*b8*/ Widget{}; // (1) extends lifetime
  Widget &&rref = /*b8*/ Widget{};       // (2) 
  // using rref here: no guarantee about state of the reffered object

  Widget return_prvalue() { return /*b8*/ Widget{}; } // copy elision
  ...
  Widget var = /*b*/ return_prvalue() /*x*/;     // (3)
  Widget &&rref2 = /*b*/ return_prvalue() /*x*/; // (4)
  take_rref(std:move(rref2));
  const Widget &rref2 = /*b*/ return_prvalue() /*x*/; // (4)
```
A materialized temporary object can 
- bind to a `const` lvalue reference `clref` (1);
- but also to a non-const rvalue reference `rref` (2).

A non-reference return type is also
- a prvalue which initializes a variable `var` directly (3)
- or materialize as temporary object when binding to a reference `rref2` (4).

> Informally, how would you characterize prvalues?
> - every function call which returns by-value is a prvalue 
> - every expression whose evaluation initializes a temporary object 
> - access of a non-static non-reference data member through an prvalue expression materialized the object and results in xvalues
> ```pmans
> Widget{}.i; // 'Widget{}.i' is xvalue
>// this is called "temporary materializaion" of a prvalue due to member access
> ```
> - binding a prvalue to a reference results in temporary materialization AND extends the lifetime to the life time of the reference
```pmans
const Widget &lref = Widget(); // makes temporary and binds to a const lvalue reference
Widget &&rref = Widget(); // makes temporary and binds to a rvalue reference
```


**xvalue** expressions determine a *locatable* `Widget` which was "tagged"(casted) as movable; i.e., its resources are allowed to be "stolen" without disturbing anyone else. In the snippet below, all xvalue expressions for type `Widget` are marked
```pmans
// what can be passed into the function?
// 1. a temporary (prvalue)
// 2. an expiring value (xvalue)
// -> this means all rvalues can bind to the argument
// -> lvalues cannot be passed (compiler error)
  void accept_rval(Widget &&rref) {   // (1)
    // rref's resources can safely be "stolen" if this is useful for us
    // without disturbing anyone 'outside'
  };
  ...
  Widget var;                              // (2)
  accept_rval(var); // error, 'var' is lvalue, cannot bind to rref
  accept_rval(Widget{}); // ok? yes! temporary is bound to argument
  accept_rval(/*b*/ std::move(var) /*x*/);            // (3)
  // 'var' is in a valid and destructible state here
  //  but resources might have been "stolen"
  var = var2; // copy-assign/reinit is possible
  // now var has known state again here
```
An rvalue reference function parameter `rref` (1) is used to bind to xvalues passed as argument.

A local variable `var` (2) is "tagged" as movable using `std::move(...)` (3) and passed as argument. 

> Example for "stealing" resources
> - `std::vector<double>` owns dynamically allocated memory. "Stealing" its resources would mean to copy the pointer to the allocated memory into another vector and reset the original pointer to "null". Of course this also applies to other stateful members which have to be adjusted for a proper "theft". A requirement is to create a valid and destructible state for the "robbed" object.

> Informally, how would you characterize xvalues?
> - handle to a locatable and movable object (e.g., temporary with extended lifetime, or explicitly "tagged" by the programmer using `std::move(...)` ) 
> - *unnamed* rvalue reference 
> - typically is bound to a rvalue reference variable 
> - caveat: a named variable of type rvalue reference is *not* an xvalue, but an lvalue


### Mixed value categories

**rvalue** expressions combine **prvalues** (materialized temporary objects with no identity) and
**xvalues** (locatable objects which are movable), i.e., all expressions which can be *moved from*. 

 The snippet below marks all rvalue expression for `Widget`:
```pmans
void accept_rval(Widget &&rref)
...
Widget var;
accept_rval(/*b*/ std::move(var) /*x*/); // (1) xvalue
accept_rval(/*b*/ Widget{} /*x*/); // (2) prvalue -> temporary -> bound by rref
```
> Why is it useful to group prvalues and xvalues together as rvalues?
 > - provide a syntactially unique way to bind to objects which can be moved from (the compiler can do different optimizations though)

**glvalue** expressions combine **xvalues** (locatable and movable) and **lvalues** (locatable and non-movable), i.e., all expressions determining a locatable object: 
```pmans
Widget var;
/*b5*/ var.i = 5;                       // (1) lvalue
Widget &&rref = /*b*/ std::move(var) /*x*/;  // (2) xvalue
/*b*/ rref.m /*x*/ = 7;                      // (3) lvalue
```

> Why is it useful to group lvalues and xvalues together as glvalues?
> - provide a syntactially unique way to bind to objects which are locatable 

After looking closely on different categories of expression we now look at references (which can bind to the objects determined by expressions) more closely.

___
next lecture proceeds here
___
## Non-const lvalue references
Non-const lvalue references can only be bound (initialized) using lvalue expressions determining non-const objects.
```pmans
  Widget var{};
  Widget var2{};
  const Widget cvar{};
  Widget &/*b4*/ lref = var;            // (1) 
  Widget &/*b4*/ lref = cvar;           // (2) 
  Widget &/*b4*/ lref = Widget{};       // (3) 
  Widget &/*b4*/ lref = std::move(var); // (4) 
```
> Which of the lines above do not compile? What are the error messages?
<!-- > - (2)-(4), see inline comments above -->

The situation for function arguments is identical:
```pmans
  void accept_lref(Widget &/*b4*/ lref) {};    
  ...
  Widget var{};
  const Widget cvar{};

  accept_lref(var);            // (1) 
  accept_lref(cvar);           // (2) 
  accept_lref(Widget{});       // (3) 
  accept_lref(std::move(var)); // (4) 
```
> Is anything different to the above examples which bind to variables instead of function argument? 
<!-- > - no -->

## Const lvalue references
If a lvalue reference is declared `const` it can also be initialized with `const` lvalues and rvalues (prvalues and xvalues) extending their lifetime.
```pmans
  void binds_clref(/*f5*/ const Widget & /*b5*/ clref) {};
  ...
  Widget var{};
  const Widget cvar{};

  binds_clref(var);            // (1) 
  binds_clref(cvar);           // (2) 
  binds_clref(Widget{});       // (3) 
  binds_clref(std::move(var)); // (4)
```
> Which of the lines compiles now? Which lifetimes are extended?
<!-- > - all lines compile, see inline comments above -->

> When is a lvalue reference to a `const` object useful?
<!-- > - its useful as it can bind to everything (see above) --->
<!-- > - ... although access is the always `const`, even if the object it was bound to was non-const. -->

## Non-const rvalue references
Non-const rvalue references can bind to non-const rvalues (xvalues and temporaries).

```pmans
  Widget var{};
  const Widget cvar{};
  Widget && /*b4*/ rref = var;            // (1) 
  Widget && /*b4*/ rref = cvar;           // (2) 
  Widget && /*b4*/ rref = Widget{};       // (3) 
  Widget && /*b4*/ rref = std::move(var); // (4) 
```
> Which lines do not compile?
<!-- > - (1)-(2), see inline comments above -->

> Why do we even need rvalue references?
<!-- > - rvalue reference form the basis for the *move semantics* introduced in C++11 -->

## Const rvalue references
If a rvalue reference is declared `const` it can bind to any rvalue (xvalues and temporaries):
```pmans
/*f5*/ const Widget &&/*b5*/ crref = Widget{};        // (1) 
/*f5*/ const Widget &&/*b5*/ crref = std::move(cvar); // (2)
```
(1) binds a const rvalue reference to a temporary object and (2) does the same for a expression determining a constant rvalue.

> Do we need const rvalue references?
<!-- > - the are not useful in the sense that they could be moved from (as the object should not be changed) -->

## `auto` type deduction 
The above rules for "which value categories can bind to which reference type" are nearly identical for the `auto` type deduction:
```pmans
  auto var = (...);           // (1) copy-ctor
  const auto cvar = (...);    // (2) again non-reference
  auto &lref = (...);         // (3) (...) binds to lvalue ref
  const auto &clref = (...);  // (4) can bind to rvalue and lvalue
  /*b7*/ auto &&fref = (...);       // (5) special functionality! "forwarding reference"
  const auto &&crref = (...); // (6) (...) is rvalue
```
- (1) initializes a non-reference variable,
- (2) initialize a const non-reference variable,
- (3) binds a non-const lvalue reference to a non-const lvalue,
- (4) binds const lvalue reference to any expression,
- (5) binds a *forwarding reference* to any expression,
- (6) binds a const rvalue reference to any rvalue expression.

With this knowledge let's try some things which do not work out with `auto`:
```pmans
  struct Widget {
    int m;
    Widget(const Widget &) = /*f*/ delete /*x*/;
  };
  Widget var{};
  auto copy /*b1*/ = var;          // (1) error: no way to copy
  const auto copy = var;    // (2) error: no way to copy
  auto &var = Widget{};     // (3) error: rhs is not an lvalue
  const auto &var = ...;    // (4) always works: rhs can be rvalue or lvalue
  /*b7*/ auto &&fref = ...;       // (5) always works
  const auto &&crref = var; // (6) error: not an rvalue
```

## Forwarding references

Let's look closer at the difference between (4) and (5) which both "always work":
- (4) always binds to a `const` lvalue reference, so it is "not-forwarding" the constness or the value category (lvalue or rvalue).
- (5) always binds a reference and does preserve the constness and value category of the object determined by the expression on the right hand side.

Some further example where the constness and value category is reflected in the reference:
```pmans
  Widget var{};
  auto &&fref = var;                            // (1)
  auto &&fref = std::as_const(var);             // (2) 
  auto &&fref = Widget{};                       // (3) 
  auto &&fref = std::move(std::as_const(var));  // (4)
```
> What is the constness and value category of the references above?
<!-- > - see above -->

As `auto &&` references preserve the original constness and value category they are also *forwarding references*.

## Reference collapsing
It is permitted to take references of references, e.g., using a type alias as in the snippet below:
```pmans
  struct Widget {
    int m;
  };
  using /*f4*/ Rref = Widget &&;
  using /*f4*/ Lref = Widget &;
  /*f4*/ Rref &&rref = Widget{};      // (1) 'rref' is of type Widget&& (no lvalue involved)
  Widget && &&rref = Widget{}; // hypothetical equivalent
  Widget var{};
  /*f4*/ Lref &&lref = var;           // (2) 'lref' is of type Widget&
  Widget & &&lref = var;       // hypothetical equivalent (lvalue involved)
  Widget & &llref = ...         // collapse to lvalue reference (lvalue involved)
``` 
This is called *reference collapsing* and the rule can be describes as: 
- Whenever one or more lvalue references `&` are involved the whole construct decays to a lvalue reference `&` (1). 
- An rvalue reference `&&` is only obtained if exclusively rvalue references `&&` are involved (2).

## Perfect forwarding
Often, a "perfect forwarding" of function parameters is highly desirable, e.g., passing them to a nested function without the nested function experiencing any difference (compared to a direct invocation). 
*Forwarding references* combined with the *reference collapsing* can achieve this.
Let's consider a function taking a single non-reference parameter `Widget`:
```pmans
Widget var;
auto func = [](Widget w){};
```
A direct invocation using an lvalue (1) and rvalue (2) looks like this:
```pmans
func(var);      // (1) call with lvalue param
func(Widget{}); // (2) call with rvalue param
```
Using forwarding references in form of `auto &&` and revealing the original value category by using `std::forward` forwards the arguments with identical constness and value category to the function:
```pmans
auto&& fref1 = var; // lvalue reference
auto&& fref2 = Widget{}; // rvalue reference
func(std::forward<decltype(fref1)>(fref1)); // (1) call with lvalue reference
func(std::forward<decltype(fref2)>(fref2)); // (2) call with rvalue reference
```
> Why is it not feasible to implement perfect forwarding without forwarding references?
<!-- > - value cat. might not be clear in advance -->
<!-- > - you would have to work with overloading (this is very verbose for multiple arguments)
```pmans
func(const T arg1) {
  nested_func(arg1); // see 'original' val cat & constness
}
func(T arg1) {
  nested_func(arg1); // see 'original' val cat & constness
}
func(const T& arg1) {
  nested_func(arg1); // see 'original' val cat & constness
}
```
-->


## Closing remarks
- We did not yet look at *move semantics* (which is the primary use case for moving things).
- We did not yet look at perfect forwarding for arbitrary arguments lists.

## Links

- Value categories: https://en.cppreference.com/w/cpp/language/value_category 
https://habr.com/ru/post/479342/ 
https://docs.microsoft.com/en-us/cpp/cpp/lvalues-and-rvalues-visual-cpp?view=vs-2019 https://www.fluentcpp.com/2018/02/06/understanding-lvalues-rvalues-and-their-references/

- Forwarding references: https://en.cppreference.com/w/cpp/language/reference#Forwarding_references

- Reference collapsing: https://en.cppreference.com/w/cpp/language/reference

- Perfect forwarding: https://www.nosid.org/cxx11-perfect-forwarding.html



