1 // item status
# Class templates (raw)
In C++ not only functions can be templated but also classes.
Let's look at a simple structure `Pair` which holds two members of the same type:
```pmans
/*f5*/ using /*b1*/ T = double;
struct /*f4*/ Pair {
  /*b1*/ T first;
  /*b1*/ T second;
  /*f4*/ Pair operator+(const /*f4*/ Pair &other) {
    return /*f4*/ Pair{first + other.first, second + other.second};
  }
};
...
  // usage
  Pair p1{1.0, 2.0};
  Pair p2{2.0, 1.0};
  auto sum = p1 + p2;
```
Assuming the implementation of `Pair` is identical for a supported range of types which are aimed to be available as a "pair", it is advantageous to make `Pair` a *class template* with a single template parameter, which can look like this:
```pmans
/*f8*/ template <typename /*b1*/ T = double> 
struct /*f4*/ Pair {
  /*b1*/ T first;
  /*b1*/ T second;
  /*f4*/ Pair operator+(const /*f4*/ Pair &other) {
    return /*f4*/ Pair{first + other.first, second + other.second};
  }
};
```
The usage of this class template is slightly different from the non-templated version above:
```pmans
... 
  { // usage with `int`
  Pair/*b*/ <int> /*x*/ p1{1, 2}; //potentially could work
  Pair/*b*/ <int> /*x*/ p2{2, 1};
  auto sum = p1 + p2;
  }  
  { // usage with `double`
  Pair/*b*/ <double> /*x*/ p1{1, 2};
  Pair/*b*/ <double> /*x*/ p2{2, 1};
  auto sum = p1 + p2;
  }

```

> Why do the template parameters have to be explicitly provided above?
> - deduction rules dont allow dedection from aggretae init

> Can this be avoided?
> - yes, by providing extra "hints"

Let's extend the `Pair` by adding an custom constructor:
```pmans
...
  /*f4*/ Pair(const /*b1*/ T &first, const /*b1*/ T &second) : first(first), second(second) {}
...
```
This allows to avoid the explicit template arguments, as the deduction can be performed using the arguments of the constructor.

## Template argument determination
As for a *function template* also for a *class template* all arguments for the template parameters must be known to the compiler.
And again (as for function templates) there are three mechanism which lead to the determination of template arguments (precedence in this order):
- explicitly specifying the arguments on construction
```pmans
  Pair</*b*/ int /*x*/> p(1, 2);    // instantiates /*f*/ Pair<int> /*x*/
  Pair</*b*/ double /*x*/> p(1, 2); // instantiates /*f*/ Pair<double> /*x*/
  Pair</*b*/ std::string /*x*/> p("1", "2"); // instantiates /*f*/ Pair<std::string> /*x*/
```
- deduction from ~~function arguments~~ constructor arguments
```pmans
  Pair p(/*b1*/ 1, /*b1*/ 2);                               // instantiates /*f*/ Pair<int> /*x*/
  Pair p(/*b*/ 1.0 /*x*/, /*b3*/ 2.0);                           // instantiates /*f*/ Pair<double> /*x*/
  Pair p(/*b*/ std::string("1") /*x*/, /*b*/ std::string("2") /*x*/); // instantiates /*f*/ Pair<std::string> /*x*/  
```
- default arguments for template parameters are defined

```pmans
  /*f8*/ template <typename /*b1*/ T = /*b6*/ double> 
  struct /*f4*/ Pair {
    ...
  }
  // usage
  Pair/*b*/ <> /*x*/ p1(1,2); // instantiates /*f*/ Pair<double> /*x*/
```

> Why is `<>` required to use the default above?
> - without the `<>` ctor arguments would be used

> What if a class has no custom constructors, can I still enable automatic deduction?
> - yes, if "deduction guide" is provided additionally

Let's look at some more situations of using the `Pair` class template where it might not be obvious which template argument is deduced:
```pmans
Pair p(/*b*/ "1" /*x*/, /*b*/ "2" /*x*/); // (1) instantiates /*f*/ Pair<char const[2]> /*x*/  c-type string
Pair p(/*b*/ "12" /*x*/, /*b*/ "34" /*x*/); // (2) instantiates /*f*/ Pair<char const[3]> /*x*/ 
Pair p(/*b*/ 1 /*x*/, /*b*/ "34" /*x*/); // (3) instantiates /*f*/ Pair<???> /*x*/ does not work 
Pair p(/*b*/ 1.0 /*x*/, /*b*/ 1 /*x*/); // (4) instantiates /*f*/ Pair<???> /*x*/ does not work
Pair p(/*b*/ 1 /*x*/, /*b*/ 1.0 /*x*/); // (5) instantiates /*f*/ Pair<???> /*x*/ same
``` 
> Can the templates above be instantiated? What are the deduced types?

> "strings":
> - `char [N]` from literal `"achararray"`
> - `char` from char literal `'c'`
> - `std::string` can be constructed from `char[N]` and `char`

```bash
# examples are in
clang++ -std=c++17 pair.cpp
```

## Template argument deduction
The rules for argument deduction are nearly identical to the rules for function templates, just that the constructor arguments now take the place of the function arguments:
```pmans
// function template
template <typename /*f4*/ AUTO> 
void func(/*b4*/ AUTO arg) {...};

// class template
template <typename /*f4*/ AUTO> 
struct Widget {
   Widget(/*b4*/ AUTO arg) {...};
     ...
}
```
The only further difference is that *forwarding references* are not available through a *class template* alone:
```pmans
// function template
template<typename /*f1*/ T>
void func(/*b3*/ T&& arg) {}; // T&& is a forwarding reference

// class template
template<typename /*f1*/ T>
struct Widget{
   T member; 
   Widget(/*f3*/ T&& arg) {}; // T&& is a rvalue reference  
void func(/*f3*/ T&& arg) {}; // T&& is a rvalue reference  
}
```
> Why does this difference make sense?
> - `T` would not be very appropriate to work with "inside the class"


So let us again recap the remaining rules for a class template with a single template parameter and a constructor with a single dependent argument.

### Pass-by-value
```pmans
template <typename /*f4*/ AUTO> 
struct Widget {
   Widget(/*b4*/ AUTO arg) {...};
     ...
}
```
- the deduced types for the template parameter and the constructor argument are identical
- `const` is dropped
- reference-ness is ignored

Let's practice a bit:
```pmans
    double lval =1.0;
    double &lref = lval;
    Widget w1(lval);                // (1) Widget<double>
    Widget w2(lref);                // Widget<double>
    Widget w3(std::as_const(lref)); // Widget<double>
    Widget w3(std::move(lref));     // Widget<double>
    Widget w3(1.0);                 // Widget<double>
```

```bash
# examples in
clang++ -std=c++17 deduce_byval.cpp
```

### Pass-by-lvalue-reference
```pmans
template <typename /*f4*/ AUTO> 
struct Widget {
   Widget(/*b*/ AUTO& /*x*/ arg) {};
};
```
- const-ness is preserved
- constructor argument is always lvalue-reference
- template parameter is never reference
- the reference-ness of the expression passed to the constructor does not influence deduction

Let's again practice a bit:
```pmans
    double lval =1.0;
    double &lref = lval;
    Widget w1(lval);                    // Widget<double>::Widget(double& arg)
    Widget w2(lref);                    // Widget<double>::Widget(double& arg)
    Widget w3(std::as_const(lref));     // Widget<const double>::Widget(const double& arg)
    Widget w4(std::move(lref));         // (4) Widget<???>::Widget(???& arg) error, requires lvalue
    Widget w5(1.0);                     // Widget<???>::Widget(???& arg) same
```
```bash
# examples in
clang++ -std=c++17 deduce_bylref.cpp
```

### Pass-by-rvalue-reference
```pmans
template <typename /*f4*/ AUTO> 
struct Widget {
   Widget(/*b*/ AUTO&& /*x*/ arg) {};
};
```
- this is **not** a forwarding reference, but a rvalue-reference
- otherwise the rules are identical to lvalue-reference (but different binding rules, of course)

Let's practice again:
```pmans
    double lval =1.0;
    double &lref = lval;
    Widget w1(lref);                            // Widget<???>::Widget(???&& arg)
    Widget w2(std::move(std::as_const(lref)));  // Widget<???>::Widget(???&& arg) 
    Widget w3(std::as_const(lref));             // Widget<???>::Widget(???&& arg)
    Widget w4(std::move(lref));                 // Widget<???>::Widget(???&& arg)
    Widget w5(1.0);                             // Widget<???>::Widget(???&& arg)
```
```bash
# examples in
clang++ -std=c++17 deduce_byrref.cpp
```

### Pass-by-reference-to-const
When using lvalue-references-to-const or rvalue-reference-to-const this looks like this:
```pmans
template <typename /*f4*/ AUTO> 
struct Widget {
   Widget(/*b*/ const AUTO& /*x*/ arg) {}; // lvalue-reference-to-const
   Widget(/*b*/ const AUTO&& /*x*/ arg) {}; // rvalue-reference-to-const
};
```
The rules are identical to the rules for references above, just that the `const` is always present in the type of the constructor argument but never in the template parameter.

```bash
# examples in
clang++ -std=c++17 deduce_bycrefs.cpp
```


## Example: `Pair` with two template parameters
A function template or class template can be defined using more than one template parameter. 
Let's modify the `Pair` class template from above to now have separate template parameters for each member:
```pmans
template <typename /*f5*/ FIRST, typename /*f6*/ SECOND> struct Pair {
  /*f5*/ FIRST first;
  /*f6*/ SECOND second;
  Pair(const /*f5*/ FIRST &first, const /*f6*/ SECOND &second)
      : first(first), second(second) {}
  auto operator+(const Pair &other) {
    return Pair{first + other.first, second + other.second};
  }
};

// usage 
Pair p1(1, 2.0); // (1) Pair<int, double> 
Pair p2(2.0, 1); // (2) Pair<double, int> 
auto sum = p1 + p2; // (3) still works ?
```
> Did anything break by introducing the second template parameter?
> If yes, how to resolve it?

```bash
# examples in
clang++ -std=c++17 pair2.cpp
```


## User defined deduction guides

Of course, also multiple constructors can be present and not necessarily all (or even none) of them allows a deduction of the template arguments:
```pmans
template <typename /*f4*/ AUTO> 
struct Widget {
  Widget(/*b4*/ AUTO arg) {};              // deduction succeeds
  Widget(/*f6*/ double arg1, /*f3*/ int arg1) {}; // deduction fails
     ...
}

// usage
Widget w(1.0); // succeeds
Widget w(1.0,2); // fails
```
In such cases (1a) a user-defined *deduction guide* (1b) can be provided to enable "automatic" deduction:
```pmans
template <typename /*f4*/ AUTO> 
struct Widget {
  Widget(/*b4*/ AUTO arg) {};              // deduction succeeds
  Widget(/*f6*/ double arg1, /*f3*/ int arg1) {}; // (1a) deduction fails
     ...
}
/*b*/ Widget(double, int) -> Widget<int>; /*x*/ // (1b) deduction guide

// usage
Widget w(1.0); // succeeds
Widget w(1.0,2); // now succeeds, too
```

```bash
# examples in
clang++ -std=c++17 deduction_guide1.cpp
```


Let's now look at an example where the constructor is additionally templated to allow construction from a range of types we have in mind. Further, we know all types we are interested in define a nested type `value_type` which we would like to set/deduce as template parameter for `Widget`:
```pmans
template <typename /*b1*/ T> 
struct Widget {
  /*b1*/ T m;
  template <typename /*b5*/ OTHER>
  Widget(const /*b5*/ OTHER &o) {} 
  // construct Widget and set T = OTHER::value_type
};
...
```
And let's further consider a second class `Other` which is one of the types we would like to use to construct a `Widget`.
Automatic deduction fails as the template parameters `T` and `OTHER` no nothing about each other: there is no relation.
Again, the solution is to create such a "custom" relation by providing a deduction guide (which is now also a template):
```pmans
...
template <typename /*b5*/ OTHER>
Widget(const /*b5*/ OTHER &o) -> Widget<typename /*b5*/ OTHER::value_type>; // deduction guide template

struct Other {
  using value_type = int;
  value_type m;
};
// usage
int main() {
  Widget w(Other{}); // will use deduction guide:
                     // Widget<int>::Widget<Other>(const OTHER& o);
}
```

```bash
# examples in
clang++ -std=c++17 deduction_guide2.cpp
```

## Class template specialization
After the declaration of a *primary* class template (all we have seen so far was primary class templates) *specializations* can be provided for a certain subset of possible intonations.
This can be a 
- *full specialization* (all template parameters are fixed for the specialization), or a
- *partial specialization* (only some of the template parameters are fixed).

Lets look at the `Pair` class template again:
```pmans
template <typename /*b1*/ T = double> 
struct /*f4*/ Pair {
  /*b1*/ T first;
  /*b1*/ T second;
  /*f4*/ Pair(const T &first, const T &second) : first(first), second(second) {}
  /*f4*/ Pair operator+(const /*f4*/ Pair &other) {
    return Pair{first + other.first, second + other.second};
  }
};
```
Let's further assume we have a type `char` which simply needs a special version of the `operator+` but we would still like to use it as a `Pair`. 
We could achieve this by providing a *full specialization* of `Pair` for `char`:
```pmans
/*f*/ template <> /*x*/ struct Pair/*f*/ < /*x*//*b4*/ char/*f*/ > /*x*/  {
  /*b4*/ char first;
  /*b4*/ char second;
  Pair(const /*b4*/ char &first, const /*b4*/ char &second) : first(first), second(second) {}
  auto operator+(const Pair &other) {
    return Pair<std::string>{
        std::string(1, first) + std::string(1, other.first),
        std::string(1, second) + std::string(1, other.second)};
  }
  
};
```
Specializations are not required to "look the same" as their associated *primary template*, it is just that they are only "found" through the primary template during name lookup. 

```bash
# examples in
clang++ -std=c++17 pair_special.cpp
```

### Example for partial specialization: is_same
Let's look an an example which uses class template specialization to implement a compile time check if two type are identical.
We start by preparing two small helper class `True` and `False` which each hold a `static const` member:
```pmans
struct False {
  static const bool value = false;
};
struct True {
  static const bool value = true;
};
```
Next we implement the primary template `is_same` which inherits from `False`:
```pmans
template <class A, class B>
struct is_same : False {}; // primary template, two parameters
```
Up to now, `is_same` is useless, if we would use it as intended:
```pmans
const bool same = is_same<double,double>::value; // always false
const bool same = is_same<double,float>::value; // always false
```
This changes when providing this partial specialization additionally:
```pmans
template <class C>
struct is_same<C, C> : True {}; // partial specialization, one parameter left
```
This partial specialization is selected whenever it "matches better" than the primary template; which is the case when `A` and `B` are identical. Now we can use `is_same` as originally intended:
```pmans
int main(){
    std::cout << is_same<double,double>::value << std::endl; // true
    std::cout << is_same<double,float>::value << std::endl; // false
    static_assert(is_same<double,float>::value);
}
```
```bash
# examples in
clang++ -std=c++17 is_same.cpp
```

> Is there such a thing as `is_same` also in the standard library?
> - yes, stdlib type traits is full of them
> - `std::is_same`

## Types of template parameters
Up to now we only considered *type template parameters*: parameters which represent a type.
There are two further categories:
- non-type template parameters (i.e., values)
- template template parameters (i.e., a template type where the parameters are not settled)

### Non-type template parameters
The most common example for a non-type template parameters are integers or boolean values:
```pmans
template<typename /*f1*/ T, /*f3*/ int /*b1*/ N>
struct /*f6*/ Widget {
    /*f1*/ T data[/*b1*/ N]; 
};
// usage
Widget</*f6*/ double,/*b2*/ 10> w10;
Widget</*f6*/ double,/*b3*/ 100> w100;
```
```pmans
template <typename /*f1*/ T, /*f4*/ bool /*b6*/ option = false> struct Widget2 {
  /*f1*/ T calculate() {
    if constexpr (/*b6*/ option) {
      return /*f1*/ T{0};
    } else {
      return /*f1*/ T{1};
    }
  }
};
// usage
Widget</*f6*/ double> wf;
Widget</*f6*/ double,/*b4*/ true> wt;
```
```bash
# examples in
clang++ -std=c++17 nttp.cpp
```

> How does the non-type template parameter compare to a simple bool member in above example?
> - performance

### Template template parameters
An example for a template template parameter is in the following snippet:
```pmans
template <typename T, /*b*/ template <typename ...> class CONTAINER /*x*/ = std::vector>
struct Widget{
  /*b*/ CONTAINER /*x*/<T,std::allocator<T>> ctnr;
};
```
This allows to use an actual template type (e.g., `std::vector` and not `std::vector<double>`) as template parameter. 

```bash
# examples in
clang++ -std=c++17 tttp.cpp
```


> In the snippet above, is it actually required to use a template template parameter?
> What is the reason to use `...` above?

## Template parameter pack 
C++ allows to specify a *template parameter pack* (e.g., `typename ...ARGS`) as template parameter which can be mapped to a *function parameter pack* (e.g., `func(ARGS&&...args)`).
This allows to construct function templates accepting a variable number of arguments:
```pmans
template<typename .../*b4*/ ARGS> // named template parameter pack
void function(/*b4*/ ARGS... /*f4*/ args) { // function parameter pack
  ... // how to access/deal with /*f4*/ args here?
}
```
Let's look at some examples to illustrate some use cases using *fold expressions*:
```pmans
template <typename... /*b4*/ ARGS> auto sum_args(/*b4*/ ARGS &&... /*f4*/ args) {
  return (0 + ... + /*f4*/ args); // sums all args, initial value '0'
};
```
```pmans
template <typename... /*b4*/ ARGS> auto print_args(/*b4*/ ARGS &&... args) {
  ((std::cout << args << ','), ..., (std::cout << std::endl)); // prints all args
};
```
> Can we also use this to wrap a function?
```pmans
template <typename... /*b4*/ ARGS> auto forward_args(/*b4*/ ARGS &&... /*f4*/ args) {
  return func(/*f4*/ args...); // passes all args to 'func' in same order
};
```
> Why is the above snippet not "perfect forwarding"?
```pmans
template <typename... /*b4*/ ARGS> auto perfect_forward_args(/*b4*/ ARGS &&... /*f4*/ args) {
  return func(std::forward</*b4*/ ARGS>(/*f4*/ args)...);
};
```
Parameter pack can also be combine with "regular" template parameters. Below this is used to build a "perfect" function wrapper:
```pmans
template <typename /*b4*/ FUNC, typename... /*b4*/ ARGS>
auto perfect_function_wrapper(/*b4*/ FUNC &&/*f8*/ callable, /*b4*/ ARGS &&... /*f4*/ args) {
  return /*f8*/ callable(std::forward</*b4*/ ARGS>(/*f4*/ args)...);
};
```
```bash
# examples in
clang++ -std=c++17 tppack.cpp
```


## Links
- Function template: https://en.cppreference.com/w/cpp/language/function_template
- Class template: https://en.cppreference.com/w/cpp/language/class_template
- Class template argument deduction: https://en.cppreference.com/w/cpp/language/class_template_argument_deduction
- Full specialization: https://en.cppreference.com/w/cpp/language/template_specialization
- Partial specialization: https://en.cppreference.com/w/cpp/language/partial_specialization
- std::is_same: https://en.cppreference.com/w/cpp/types/is_same
- Fold expression: https://en.cppreference.com/w/cpp/language/fold
- Parameter pack: https://en.cppreference.com/w/cpp/language/parameter_pack
