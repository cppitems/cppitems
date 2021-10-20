1 // item status
# Overview: types and expressions

> What is the purpose of today's lecture?
> - develop some experience how to look at C++ code: w.r.t Types, Expressions

Looking at a certain portion of C++ source code it is not immediately apparent what the consequences are exactly:
  
```pmans
  using Type = ...;
  Type b{1};                            // (1)
  Type a = b;                           // (2)
  auto c = a + b + b++;                 // (3)
```
> What is the state of `b` after (1)?
> - initialized with `1` (int) but we know nothing more 

> How are `a` and `b` related after (2)? 
> - `a` points to `b`? no:`a` is not a reference type (but it might be the case that member of `b` are related to members of `a`)
> - depends on assingment operator?: yes
> - shallow copy? maybe: depends on the implementation of the copy-constructor

> Does (3) have side effects on `a`? What about `b`?
> - side effects on `a`? no, beside if (2) keeps a relation, e.g., between members
> - side effects on `b`? yes: `b++`
> - other effects? maybe: implementation of `operator+(...)` between `Widgets` is unknown

To answer these questions, it is required to know details about the involved *types* in an *expression* and how expressions are evaluated. If `using Type = int;` the consequences of the above statements/expressions are intuitively clear. 
But what about this:
```pmans
  using Type = int;
  auto d = 2 + 2 * Type{1} / Type(2.0); // (4)  any difference between {} and ()?
  auto d = 2 + 2 * Type{1} / Type(2.0); //
  Type{2.0} // more strict w.r.t. conversions, i.e., no truncating implicit convertions
  Type(2.0) // allows implicit conversions, i.e., also truncations
  auto e = 2.5 + Type{1} / Type{2} * 5; // (5)
  auto e = DOUBLE + INT; // (5) default is resulting in non-truncating type
```
To exactly describe the effects of (4) and (5) additional knowledge about the `auto` *type deduction* mechanism, involved operators/precedence, and *implicit conversions* of *fundamental types* is required:

```pmans
  /*b3*/ int d = 2 + /*b2*/ ((2 * 1/*b1*/ ) / 2 /*b1*/ ); // (4) equivalent
  /*b6*/ double e = 2.5 + /*f*/ static_cast<double> /*x*//*b3*/ (((1 / 2/*b1*/ ) * 5/*b2*/ )); // (5) equivalent
```

```bash
# show deduced types, conversions and precedence
insights 01_fundamental_types.cpp -- -std=c++17 
# dump clang AST of above snippet
clang-check -extra-arg=-std=c++17 -ast-dump --ast-dump-filter=main 01_fundamental_types.cpp --
```

> Some more examples for common fundamental types / common user-defined types?
> - `double`, `float`, `long`, `int`, `char`, `bool`
> - `std::numeric_limits`, `std::vector`, `std::set`, `std::iterator`, `std::map`, `std::shared_ptr`, `std::enable_if`, `std::move`, `std::thread`

## Expressions
An *expression* describes a computation by prescribing a sequence of operations to be performed on a set of operands. For the fundamental type `int`, some expressions where the effect of *operators* can be guessed are:
```pmans
    using Type = int;
    Type a{1};
    Type b{2};
    Type c{3};
    a /*b1*/ = b /*b1*/ + c /*b1*/ + c;                             // (1)
    a /*b1*/ = b /*b1*/ + c /*b1*/ * c;                             // (2)
    a /*b1*/ = 2 /*b1*/ + c /*b1*/ + 1 /*b1*/ / 2;                         // (3)
    a /*b1*/ = 2.5 /*b1*/ + c /*b1*/ + a;                           // (4)
    a /*b1*/ + b /*b1*/ + c;                                 // (5)
```
Typically, *operands* are variables and most operators are *binary*, i.e., work with two operands. 
Transforming (2) into equivalent forms which make this visible looks like this:
```pmans
    a = (b + (c * c));                             // (2) equivalent
    a./*f9*/ operator=/*b1*/ (b./*f9*/ operator+/*b1*/ (c./*f9*/ operator*/*b1*/ (c/*b3*/ )));  // (2) conceptually equivalent
    a /*b1*/ = b /*b1*/ + c /*b1*/ + c;                              // (2)
    a.operator=(ONE) // binary operator (but only one argument)
    a.operator=(c.operator*(c)) // 
    a.operator=(b.operator+(c.operator*(c))) // 
```

> Examples for non-binary operators?
> - NOT operator: `!(expr)` -> unary
> - in-place increment and  decrement operators: `++` `--` -> unary
> - bitshift operations: `a << 2;` -> binary
> - "inline if-else": `condition ? a : b` -> ternary


Function calls can also  be part of an expression (i.e., the returned value):
```pmans
    auto /*f*/ lambda /*x*/ = [&a](Type b) { return /*b2*/ --a /*b1*/ - b; };  // (6)
    // what does the lambda above do for us: 
    // allows to define an inline "function object" with "captures" 
    a /*b1*/ = /*f*/ lambda /*x*/(b) /*b1*/ + b /*b1*/ - /*f*/ lambda /*x*/(c);                   // (7) associativity, precedence 
```
The *order of evaluation* of sub-expressions is not defined; but the operator precedence defines which operands are associated with each operator. 

> Which of the two `lambda` evaluations takes place first ?
>- the order of evaluation is *not* defined in the standard

> What happens if multiple operators of equal precedence are 'chained'?
 > - they are evaluated accoring to the associativity of the operator 
>```pmans
>a + b + c; // 'chained' operator+; operator+ is left-to-right associative
>((a + b) + c); // equivalent with explicit parentheses
>```
> - for (7) above explicitly:
> ```pmans
> auto tmp = lambda(b) + b; // 1) leftmost subexpression due to left-to-right associativity
> a = tmp - lambda(c);      // 2) combining `tmp` from 1) and third operand
> ```


To represent intermediate results of sub-expressions, *temporary objects* might be created during execution of an expression. The lifetime of temporary objects ends with the *full-expression*: after evaluation of the expression, all temporary objects are destroyed.

The following commands can be used to gain detailed insights on the interpretation of the snippet above:
```bash
# show deduced types, conversions and precedence
insights 02_expressions.cpp -- -std=c++17 
# dump clang AST of above snippet
clang-check -extra-arg=-std=c++17 -ast-dump --ast-dump-filter=main 02_expressions.cpp --
```

## Types
Every *entity* that is used in an *expression* has a *type*; also the expressions themselves.
This type controls how an entity can be "used" and what it exactly means when it is used in a certain semantic embedding (i.e., operators in an expression).
*All* types are determined at compile time, even if they are not explicitly visible in the source (using deduction rules).

> What are a typical compile time errors w.r.t. types?  
> - conversions
> - access to non-existing "things"
> - trying to modify objects of `const` type
> - nearly all error messages are related to types

> Which run time errors are possible in dynamically typed languages, e.g., JavaScript or Python?
> - JavaScript defaults to `undefined` if a method or member does not exist and run time checking might look like:
>```pmans
>// JavaScript
>function func(object) {
>  if(object.method === undefined ) { return object.method(5); }
>  else { throw "error"; }
>}
>```
> - to provide type safety for JavaScript or Python, compile time type checking frameworks (e.g., mypy or TypeScript) are available, releasing the burden to do checks like the one above

### Debugging types

To reveal a type which is not straightforwardly visible at compile time (i.e., a deduced type, or the type of a sub-expression), the compiler can be tricked to return an error involving the type. This error is triggered by trying to resolve a non-existing type/member:

```pmans
template <typename T> struct DebugType { using T::notexisting;};
// usage: DebugType<decltype(...)> error;

int main() {
  const int &a = 5;
  const double b = a + 5.0;
  DebugType<decltype(...)> type_a;
  ...
}
```
This will reveal the type of interest somewhere in the compiler's error message:
```pmans
/*f*/ gcc /*x*/:
debug_type.cpp: In instantiation of 'struct DebugType<const int&>':
debug_type.cpp:14:26:   required from here
debug_type.cpp:1:51: error: type '/*b*/ const int& /*x*/' is not a base type for type 'DebugType</*b*/ const int& /*x*/>'
 template <typename T> struct DebugType { using T::notexisting;};
                                                   ^~~~~~~~~~~ 
...
/*f*/ clang /*x*/:
debug_type.cpp:1:48: error: type '/*b*/ const int& /*x*/' cannot be used prior to '::' because it has no members
template <typename T> struct DebugType { using T::notexisting;};
                                               ^
debug_type.cpp:14:26: note: in instantiation of template class 'DebugType</*b*/ const int& /*x*/>' requested here
  DebugType<decltype(a)> type_a;
```

```bash
# compile with errors revealing certain types
clang++ -std=c++17 03_debug_type.cpp
```

Let's now look at the snippet below and try to guess the involved types as good as possible. 
The snippet includes:
- a template function definition (1) and its *explicit instantiation* (3)
- many `auto` deduced types including lambda functions (10) (12)
- types defined using `decltype` (8) (15)

```pmans
#include <initializer_list>
#include <tuple>

template <typename AUTO> void func(const AUTO &expr){}; // (1)
int main() {                                            // (2)
  auto /*b*/ func_int /*x*/ = func<int>;                           // (3)
  auto /*b1*/ a = 2;                                           // (4)
  const auto /*b1*/ b = 2.0;                                   // (5)
  double /*b1*/ c = a + 2;                                     // (6)
  auto /*b1*/ d = {2};                                         // (7)
  decltype(d) /*b*/ e /*x*/;                                        // (8)
  struct Widget {
    int i;
  };                                                          // (9)
  auto /*b*/ lambda /*x*/ = [=](Widget w) { return w.i + a + b; };       // (10)
  // lambda adv 1: cannot define inline function here
  // lambda adv 2: lambda can "capture the local surrounding"
  // what does auto do for us here?
  //struct Lambda {
  //    // implement all the things so it behaves as "expected"
  //};
  //Lambda lambda(...);
  auto /*b*/ res /*x*/ = lambda(Widget{2});                               // (11)
  auto /*b7*/ lambda2 = [&]() { return std::make_tuple(a, b, c); }; // (12)
  auto [/*b*/ one /*x*/, /*b3*/ two, /*b5*/ three] = lambda2();                        // (13)
  auto /*b3*/ str = "C++";                                           // (14)
  decltype("C++") /*b1*/ f{};                                        // (15)
  return 0;
}
```

To reveal the actual types we can use the method introduced above. 

> Side topic: why are `lambda` functions actually attractive?
> - what are the actual alternatives: 
>   - 1) free standing functions -> cannot be defined "inline" and cannot "capture"
>   - 2) classes with user defined constructors and overloaded `operator=(...)` -> cannot be defined "inline"

### Types produced by "high level" language features

For lambda functions, the compiler error message might not be very informative. E.g., for line (10) the message looks like this:

```pmans
... error: using declaration refers into '(lambda at more_types.cpp:18:17)::',
 which is not a base class of 'DebugType</*b*/ (lambda at more_types.cpp:18:17) /*x*/>'
```

Instead the following commands can be used to provide further details:

```bash
# show deduced types, conversions and precedence
insights 04_more_types.cpp -- -std=c++17 
# dump clang AST of above snippet
clang-check -extra-arg=-std=c++17 -ast-dump --ast-dump-filter=main 04_more_types.cpp --
```


For example the lambda function (10) is reported as an inline class `__lambda_18_17` and an immediate instantiation, which conceptually looks like 
```pmans
// surrounding
auto lambda = [=](Widget w) { return w.i + a + b; };       // (10) this is unfolded
class /*f*/ __lambda_18_17 /*x*/{
public:
  /*f*/ __lambda_18_17 /*x*/(int a, const double b) : a{a}, b{b} {} // constructor
  inline double /*b*/ operator() /*x*/(Widget w) const { 
    return (static_cast<double>(w.i + a)) + b;
  }
private: // local variables captured by copy [=]
  int a;
  const double b;
};
/*f*/ __lambda_18_17 /*x*/ /*b6*/ lambda = /*f*/ __lambda_18_17 /*x*/{a, b}; // instantiation
lamdba(Widget{}); // possible usage
```
which implements the behavior of the lambda function definition according the the standard.  
The decomposition in (13) (*structural binding declaration*) is reported as three individual assignments which conceptually looks like this:
```pmans
  using /*f*/ TupleType /*x*/ = std::tuple<int, double, double>;
  /*f*/ TupleType /*x*/ /*b*/ pack /*x*/ = lambda2.operator()();
  std::tuple_element<0UL, /*f*/ TupleType /*x*/>::type& /*f*/ one /*x*/ = std::get<0UL>(/*b*/ pack /*x*/);
  std::tuple_element<1UL, /*f*/ TupleType /*x*/>::type& /*f*/ two /*x*/ = std::get<1UL>(/*b*/ pack /*x*/);
  std::tuple_element<2UL, /*f*/ TupleType /*x*/>::type& /*f*/ three /*x*/ = std::get<2UL>(/*b*/ pack /*x*/);
``` 

## User-defined types

A *class type* is always user-defined: 
this means full control (and responsibility) of how his class type behaves in all possible semantic embeddings. 
Above we already encountered user-defined types from the standard library and the *inline classes* tailored to represent lambda function objects. 

Let's now look at the example from above again and change `Type` from `int` to a simple structure type with a single `int` data member and observe the consequences:
```pmans
  {
    // using Type = /*f*/ int /*x*/;
    struct /*f*/ Widget /*x*/ {
      /*b*/ int i /*x*/;
    }       
    using Type = /*f*/ Widget /*x*/;
    Type a{};
    Type b{};
    Type c{};
    a = b /*b1*/ + c /*b1*/ + c;                          // (1) 
    a = b /*b1*/ + c /*b1*/ * c;                          // (2) 
    a = 2 /*b1*/ + c + /*b1*/ ( 1 / 2 /*b1*/ );                 // (3) 
    a = 2.5 /*b1*/ + c /*b1*/ + a;                       // (4) 
    a /*b1*/ + b /*b1*/ + c;                                          // (5) 
    auto lambda = [](Type a, Type b) { return a /*b1*/ - b; }; // (6) 
    a = lambda(a, c) /*b1*/ + b;                               // (7) 
  }
```
> Which expressions would not compile? Which sub-expressions exactly?

> How to make `Widget` "compatible" with the expressions above? 

```bash
# compile
clang++ -std=c++17 -ferror-limit=1 05_expressions_user.cpp
```

# Summary

- Operator precedence defines how a source code representation of an expression is mapped to nested operator calls and what the respective operands are.

- Temporary objects which represent evaluations of sub-expressions are destroyed after the full-expression is evaluated.

- Most operators are defined for fundamental types; additionally implicit conversions exist between integral and floating point types. 

- For user-defined types, there is no default behavior in many semantic embeddings. Defaults for construction(1)(2)/assignment(3)/destruction(4) are implicitly defined if they do not interfere with user-defined details of the class.

- To inspect the exact types deduced by the compiler, an access to a non-existing member/type can be used to trigger an error revealing the type of an object.


- Some "high level" language features (we saw lambda function objects and structural binding) can be transformed to code using "lower level" language features.

## Links
- Expressions: https://en.cppreference.com/w/cpp/language/expressions
- Types: https://en.cppreference.com/w/cpp/language/type
- Operator precedence/associativity: https://en.cppreference.com/w/cpp/language/operator_precedence
- Initialization: https://en.cppreference.com/w/cpp/language/initialization
- Structured binding: https://en.cppreference.com/w/cpp/language/structured_binding
- Special member functions: https://en.cppreference.com/w/cpp/language/rule_of_three