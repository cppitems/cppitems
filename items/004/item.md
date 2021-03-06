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
  w.i = 5;                   // (2) 'w.i' is lvalue, '5' is rvalue
  5 = w.i;                   // (3) ERROR, '5' is rvalue
  return_tmp() = (Widget){5};// (4) ERROR, 'return_tmp()' is rvalue
  return_ptr()->i = 5;       // (5) 'return_ptr()->i' is lvalue
```
Let's look at a similar same code snippet again in C++ when also using references:
```pmans
/* compile: clang++ -std=c++17 */
struct Widget { int i; };
Widget gw = {1}; // global widget
Widget return_tmp() { return Widget{2}; }
Widget &return_ref() { return gw; }
void take_rref(Widget &&rref) {}
  ...
  Widget w{3};              // (0) init, from list
  Widget w2 = return_tmp(); // (1) init, from prvalue 'return_tmp()'
  w.i = 5;                  // (2) 'w.i' is lvalue, '5' is prvalue
  return_tmp() = Widget{4}; // (3) 'return_tmp()' is prvalue (temporary)
  int i = return_tmp().i;   // (4) 'return_tmp().i' is a xvalue
  return_ref().i = 5;       // (5) 'return_ref().i' is lvalue
  return_ref() = Widget{5}; // (6) 'return_ref()' is lvalue
  take_rref(std::move(w));  // (7) 'std::move(w)' is xvalue

```
Closely looking at the line comments above we can see that there are three value categories mentioned:  
- *lvalue*
- *prvalue*
- *xvalue*

And additionally there are *lvalue references* and *rvalue references* in use.
References can bind to expressions providing an alias to the object determined by the evaluation of the expression:
- rvalue references (`Widget &&`) can bind to xvalue and prvalue expression, i.e., expressions determining a moveable object).
- lvalue references (`Widget &`) can bind to lvalue expressions, i.e., expression which determine a non-temporary object which is non-moveable.

## Value categories of expressions

Let's solely consider value categories w.r.t. a `Widget` type  with a single `int` member
```pmans
struct /*f6*/ Widget {
  int /*f1*/ m;
};
```
**lvalue** expressions determine a *locatable* *non-moveable* object of the type `Widget`. 
All lvalue expressions determining a `Widget` object are marked in the snippet below:
```pmans
  Widget var;
  /*b5*/ var.m = 5;                      // (1)
  Widget /*f5*/ &lref = /*b3*/ var;             // (2)
  Widget *ptr = &/*b4*/ lref;            // (3)
  /*b4*/ *ptr = var;                     // (4)
  const Widget /*f6*/ &clref = Widget(); // (5)
  int i = /*b8*/ clref.m;                // (6)
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
>ref = other; // (1) this line does not reassign; it it equivalent to the next line
> w = opther; // (1) equivalent assignment
>```
> - when saying "const reference" this is typically a short form for "reference providing `const` access to an object", e.g., (1) is ment and not (2)
>```pmans
>Widget w{};
>const Widget& cref = w; // (1) cref provides an alias for const access to w
>Widget &const = w; // (2) this would be a const reference, providing non-const access >to w
>```
> - a consequence of the above is that references cannot be set to "null" 
>```pmans
>Widget w{};
>Widget &ref = w;
>ref /*b1*/ = nullptr; // ERROR
>```
> - ... but they can be "dangling" if the alias object is not available >anymore
>```pmans
>int& return_dangling_ref() {
>    Widget w{};
>    return w,i;
>}
> ...
> int& /*b*/ dangling /*x*/ = return_dangling_ref();
>```

> Informally, how would you characterize lvalues?
> - as expressions which provide a handle to an object stored somewhere in memory
> - the object dertermine is not a temporay object but an object with clear expectations on its lifetime and state.


**prvalue** expressions can initialize a `Widget` object but are not (yet) affiliated with locatable `Widget` object.  Depending on the context, a prvalue can be a temporary object or directly initialize a variable. In the following all prvalue expressions for `Widget` are marked:
```pmans
  const Widget &clref = /*b8*/ Widget{}; // (1)
  Widget &&rref = /*b8*/ Widget{};       // (2)
  auto return_prvalue = []() -> Widget { return /*b8*/ Widget{}; }; // copy elision
  Widget var = /*b*/ return_prvalue() /*x*/;     // (3)
  Widget &&rref2 = /*b*/ return_prvalue() /*x*/; // (4)
```
A materialized temporary object can bind to a `const` lvalue reference `clref` (1);
but also to a non-const rvalue reference `rref` (2).
A non-reference return type is also a prvalue which initializes a variable `var` directly (3) or materialize as temporary object when binding to a reference `rref2` (4).

> Informally, how would you characterize prvalues?
> - every function call which returns by-value is a prvalue
> - every expression whose evaluation initalizes a temporary object
> - access of a non-static non-reference data member through an rvalue expression are prvalues

**xvalue** expressions determine a *locatable* `Widget` which was "tagged"(casted) as moveable; i.e., its resources are allowed to be "stolen" without disturbing anyone else. In the snippet below, all xvalue expressions for type `Widget` are marked
```pmans
  auto accept_rval = [](Widget &&rref) {   // (1)
    // rref's resources can safely be stolen
    // without disturbing anyone 'outside'
  };
  Widget var;                              // (2)
  accept_rval(var); // error, 'var' is lvalue, cannot bind to rref
  accept_rval(Widget{}); // ok? yes! temporary is bound to argument
  accept_rval(/*b*/ std::move(var) /*x*/);           // (3)
  // 'var' is in a valid and destructible state here
  //  but resources might have been "stolen"
```
An rvalue reference function parameter `rref` (1) is used to bind to xvalues passed as argument.
A local variable `var` (2) is "tagged" as movable using `std::move(...)` (3) and passed as argument. 

> Example for "stealing" resources
> - `std::vector<double>` owns dynamically allocated memory. "Stealing" its resources would mean to copy the pointer to the allocated memory into another vector and reset the original pointer to "null". Of course this also applies to other stateful members which have to be adjusted for a proper "theft". A requirement is to create a valid and destructible state for the "robbed" object.

> Informally, how would you characterize xvalues?
> - handle to a locatable and moveable object (e.g., temporary with extended lifetime, or explicitly "tagged" by the programmer using `std::move(...)` ) 
> - *unnamed* rvalue reference
> - typically is bound to a rvalue reference variable
> - caveat: a named variable of type rvalue reference is *not* an xvalue, but an lvalue

### Mixed value categories

**rvalue** expressions combine **prvalues** (materialized temporary objects with no identity) and
**xvalues** (locatable objects which are moveable), i.e. all expressions which can be moved from. 
 The snippet below marks all rvalue expression for `Widget`:
```pmans
Widget var;
accept_rval(/*b*/ std::move(var) /*x*/); // (1) xvalue
accept_rval(/*b*/ Widget{} /*x*/); // (2) prvalue -> temporary -> bound by rref
```
> Why is it useful to group prvalues and xvalues together as rvalues?
> - provide a handle to a object which can be moved from

**glvalue** expressions combine **xvalues** (locatable and moveable) and **lvalues** (locatable and non-moveable), i.e., all expressions determining a locatable object: 
```pmans
Widget var;
/*b5*/ var.i = 5; // (1) lvalue
Widget &&rref = /*b*/ std::move(var) /*x*/;// (2) xvalue
/*b*/ rref.m /*x*/ = 7; // (3) lvalue
```

> Why is it useful to group lvalues and xvalues together as glvalues?
> - determine the identity of an object, i.e., the object is locatable.

After looking closely on different categories of expression we now look at references (which can bind to the objects determined by expressions) more closely.

## Non-const lvalue references
Non-const lvalue references can only be bound (initialized) using lvalue expressions determining non-const objects.
```pmans
  Widget var{};
  Widget var2{};
  const Widget cvar{};
  Widget &/*b4*/ lref = var;            // (1) yes, works
  Widget &/*b4*/ lref = cvar;           // (2) no, is lvalue, but const -> no non-const alias is expected
  Widget &/*b4*/ lref = Widget{};       // (3) no, Widget{} is rvalue (prvalue)
  Widget &/*b4*/ lref = std::move(var); // (4) no, 'var' is casted to rvalue (xvalue), cannot assign lvalue ref
```
> Which of the lines above do not compile? What are the error messages?
> - (2)-(4), see inline comments above

The situation for function arguments is identical:
```pmans
  Widget var{};
  const Widget cvar{};
  auto accept_lref = [](Widget &/*b4*/ lref) {};
  accept_lref(var);            // (1) yes ...
  accept_lref(cvar);           // (2) no ...
  accept_lref(Widget{});       // (3) no ...
  accept_lref(std::move(var)); // (4) no ...
```
> Is anything different to the above examples which bind to variables instead of function argument? 
> - no

## Const lvalue references
If a lvalue reference is declared `const` it can also be initialized with `const` lvalues and rvalues (prvalues and xvalues) extending their lifetime.
```pmans
  Widget var{};
  const Widget cvar{};
  auto binds_clref = [](/*f5*/ const Widget & /*b5*/ clref) {};
  binds_clref(var);            // (1) works
  binds_clref(cvar);           // (2) works, "both" are const
  binds_clref(Widget{});       // (3) works, lifetime is extended 
  binds_clref(std::move(var)); // (4) works, lifetime not extended by binding to argument (lifetime is bound to original variable 'var')
```
> Which of the lines compiles now? Which lifetimes are extended?
> - all lines compile, see inlin comments above

> When is a lvalue reference to a `const` object usefull?
> - its useful as it can bind to everything (see above)
> - ... although that the access is the always `const`, even if the object it was bound to was non-const.
## Non-const rvalue references
Non-const rvalue references can bind to non-const rvalues (xvalues and temporaries).

```pmans
  Widget var{};
  const Widget cvar{};
  Widget && /*b4*/ rref = var;            // (1) no, var is a lvalue
  Widget && /*b4*/ rref = cvar;           // (2) no, cvar is lvalue
  Widget && /*b4*/ rref = Widget{};       // (3) yes, Widget{} -> temporary -> bound and lifetime extended
  Widget && /*b4*/ rref = std::move(var); // (4) yes, explicitly casted to rvalue
```
> Which lines do not compile?
> - (1)-(2), see inline comments above

> Why do we even need rvalue references?
> - rvalue reference for the basis for the *move semantics* introduced in C++11 
> end of lecture 22th oct

## Const rvalue references
If a rvalue reference is declared `const` it can bind to any rvalue (xvalues and temporaries):
```pmans
/*f5*/ const Widget &&/*b5*/ crref = Widget{};        // (1) 
/*f5*/ const Widget &&/*b5*/ crref = std::move(cvar); // (2)
```
(1) binds a const rvalue reference to a temporary object and (2) does the same for a expression determining a constant rvalue.
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
  auto &&fref = var;                            // (1) Widget &fref = ...;
  auto &&fref = std::as_const(var);             // (2) const Widget &fref = ...
  auto &&fref = Widget{};                       // (3) Widget &&fref = ...
  auto &&fref = std::move(std::as_const(var));  // (4) const Widget &&fref = ...
```
> What is the constness and value category of the references above?
> - see above

As `auto &&` references preserve the original constness and value category they are also *forwarding references*.

## Reference collapsing
It is permitted to take references of references, e.g., using a type alias as in the snippet below:
```pmans
  struct Widget {
    int m;
  };
  using /*f4*/ Rref = Widget &&;
  using /*f4*/ Lref = Widget &;
  /*f4*/ Rref &&rref = Widget{};      // (1) 'rref' is of type Widget&&
  Widget && &&rref = Widget{}; // hypothetical equivalent
  Widget var{};
  /*f4*/ Lref &&lref = var;           // (2) 'lref' is of type Widget&
  Widget & &&lref = var;       // hypothetical equivalent
  Widget & &llref = ... // collapse to lvalue reference 
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
> - value cat. might not be clear in advance
> - you would have to work with overloading (this is very verbose for multiple arguments)
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



