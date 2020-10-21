9 // item status
# Overview: value categories and references
Additionally to a *type*, each expression is also characterized by its *value category* which is either *prvalue*, *xvalue*, or *lvalue*.
Compatible references can bind to expressions providing an alias to the object determined by the evaluation of the expression.

## Value categories of expressions

Let's solely consider these categories w.r.t. a `Widget` type  with a single `int` member
```pmans
struct /*f6*/ Widget {
  int /*f1*/ m;
};
```
**lvalue** expressions determine a *locatable* *non-moveable* `Widget`. 
All lvalue expressions determining a `Widget` are marked in the snippet below:
```pmans
  Widget var;
  /*b5*/ var.m = 5;                      // (1)
  Widget /*f5*/ &lref = /*b3*/ var;             // (2)
  Widget *ptr = &/*b4*/ lref;            // (3)
  /*b4*/ *ptr = var;                     // (4)
  const Widget /*f6*/ &clref = Widget(); // (5)
  int i = /*b8*/ clref.m;                // (6)
 ```
Typically lvalues are variables `var` (1)(2) or *lvalue references* `lref` (3). 
A pointer `ptr` itself is not determining a `Widget`;
but a dereferenced pointer `*ptr` (4) does.
A temporary object can bind to a `const` lvalue reference `clref` (5);
this binding extends the lifetime by means of the lifetime of the reference variable `clref` (6).

> Informally, how would you characterize lvalues?

**prvalue** expressions can initialize a `Widget` but are not (yet) affiliated with locatable `Widget` object.  Depending on the context a prvalue can materializing as a temporary object or directly initializing a variable. In the following all prvalue expressions for a `Widget` are marked:
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

**xvalue** expressions determine a *locatable* `Widget` which was "tagged"(casted) as moveable; i.e., its resources are allowed to be "stolen" without disturbing anyone else. In the snippet below, all xvalue expressions for type `Widget` are marked
```pmans
  auto accept_rval = [](Widget &&rref) {   // (1)
    // rref's resources can safely be stolen
    // without disturbing anyone 'outside'
  };
  Widget var;                              // (2)
  accept_rval(/*b*/ std::move(var) /*x*/);           // (3)
  // 'var' is in a valid and destructible state here
  //  but resources might have been "stolen"
```
An rvalue reference function parameter `rref` (1) is used to bind to xvalues passed as argument.
A local variable `var` (2) is "tagged" as movable using `std::move(...)` (3) and passed as argument. 

> Informally, how would you characterize xvalues?

### Mixed value categories

**rvalue** expressions combine **prvalues** (materialized temporary objects with no identity) and
**xvalues** (locatable objects which are moveable), i.e. all expressions which can be moved from. 
 The snippet below marks all rvalue expression for `Widget`:
```pmans
Widget var;
accept_rval(/*b*/ std::move(var) /*x*/); // (1) xvalue
accept_rval(/*b*/ Widget{} /*x*/); // (2) prvalue
```
> Why is it useful to group prvalues and xvalues together as rvalues?

**glvalue** expressions combine **xvalues** (locatable and moveable) and **lvalues** (locatable and non-moveable), i.e., all expressions determining a locatable object: 
```pmans
Widget var;
/*b5*/ var.i = 5; // (1) lvalue
Widget &&rref = /*b*/ std::move(var) /*x*/;// (2) xvalue
/*b*/ rref.m /*x*/ = 7; // (3) lvalue
```

> Why is it useful to group lvalues and xvalues together as glvalues?

After looking closely on different categories of expression we now look at references (which can bind to the objects determined by expressions) more closely.

## Non-const lvalue references
Non-const lvalue references can only be bound (initialized) using lvalue expressions determining non-const objects.
```pmans
  Widget var{};
  const Widget cvar{};
  Widget &/*b4*/ lref = var;            // (1)
  Widget &/*b4*/ lref = cvar;           // (2)
  Widget &/*b4*/ lref = Widget{};       // (3)
  Widget &/*b4*/ lref = std::move(var); // (4)
```
> Which of the lines above do not compile? What are the error messages?

The situation for function arguments is identical:
```pmans
  Widget var{};
  const Widget cvar{};
  auto accept_lref = [](Widget &/*b4*/ lref) {};
  accept_lref(var);            // (1)
  accept_lref(cvar);           // (2)
  accept_lref(Widget{});       // (3)
  accept_lref(std::move(var)); // (4)
```
## Const lvalue references
If a lvalue reference is declared `const` it can also be initialized with `const` lvalues and rvalues (prvalues and xvalues) extending their lifetime.
```pmans
  Widget var{};
  const Widget cvar{};
  auto accept_lref = [](/*f5*/ const Widget & /*b5*/ clref) {};
  accept_lref(var);            // (1)
  accept_lref(cvar);           // (2)
  accept_lref(Widget{});       // (3)
  accept_lref(std::move(var)); // (4)
```
> Which of the lines compiles now? Which lifetimes are extended?

## Non-const rvalue references
Non-const rvalue references can bind to and non-const rvalues (xvalues and temporaries).
```pmans
  Widget var{};
  const Widget cvar{};
  Widget && /*b4*/ rref = var;            // (1)
  Widget && /*b4*/ rref = cvar;           // (2)
  Widget && /*b4*/ rref = Widget{};       // (3)
  Widget && /*b4*/ rref = std::move(var); // (4)
```
> Which lines do not compile?

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
  auto var = (...);           // (1) 
  const auto cvar = (...);    // (2) 
  auto &lref = (...);         // (3) 
  const auto &clref = (...);  // (4) 
  /*b7*/ auto &&fref = (...);       // (5) special functionality! "forwarding reference"
  const auto &&crref = (...); // (6) 
```
(1) initializes a non-reference variable, 
(2) initialize a const non-reference variable, 
(3) binds a non-const lvalue reference to a non-const lvalues, 
(4) binds const lvalue reference to any expression, 
(5) binds a *forwarding reference* to any expression,
(6) binds a const rvalue reference to any rvalue expression.

With this knowledge let's try some things which do not work out with `auto`:
```pmans
  struct Widget {
    int m;
    Widget(const Widget &) = /*f*/ delete /*x*/;
  };
  Widget var{};
  auto copy = var;          // (1) error: no way to copy
  const auto copy = var;    // (2) error: no way to copy
  auto &var = Widget{};     // (3) error: not an lvalue
  const auto &var = ...;    // (4) always works
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


As `auto &&` references preserve the original constness and value category they are also called *forwarding references*.

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



