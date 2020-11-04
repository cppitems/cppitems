9 // item status
# Lambda expressions
We have already used "lambdas" in previous items. 
Let's now look at *lambda expressions* in detail.

A *lambda expression* constructs a *function object*,i.e., a class with an overloaded `operator()`, which is able to capture variables which are valid at the source location where the lambda expression is evaluated. 
This *function object* is of unnamed type is then ready to be invoked "like a function".
The required parameters for the invocation are specific within the lambda expression.
The invocation will execute the code block in the body of the lambda expression.
Let's look at a simple example where the full lambda expression and the variable for the function object (`lambda`) are marked
```pmans
{ 
  int i{};
  Widget w{};  
  auto /*f6*/ lambda = /*b*/ [i, &w] (int b) -> int { return i + w.i + b; } /*x*/; 
  int res = lambda(4);
} 
```
The effect of the above code snippet can nicely be described by mimicking the behavior using an inline class.
But before that, let's introduce some nomenclature for lambda expressions:

The **body** holds the block of code which is executed when invoking the function object.
```pmans
  ...
  auto /*f6*/ lambda = [i, &w] (int b) -> int { /*b*/ return i + w.i + b; /*x*/ }; 
  ...
  int res = /*f6*/ lambda(4); // execution/invokation of function object
```
> What is the maximum length for the code block in the body of a lambda? 

The **capture clause** lists which variables of the "outside scope" are "made available" also inside the **body**:
```pmans
  int /*b1*/ i{};
  Widget /*b1*/ w{}; 
  auto lambda = [/*b1*/ i, /*b2*/ &w] (int b) -> int { return /*b1*/ i + /*b1*/ w.i + b; }; 
  ...
```
> Can the variable names be alerted during capturing?

> What is the effect of using `&w` compared to `w` in the capture clause?

The **parameter list** list the parameter which are required when invoking the function object; they are also available in the body (as for any normal function):
```pmans
  ...
  auto lambda = [i, &w] (int /*b1*/ b) -> int { return i + w.i + /*b1*/ b; }; 
  int res = lambda(/*b1*/ 4);
```
The **return type** specifies the return type of the invocation:
```pmans
  ...
  auto lambda = [i, &w] (int b) -> /*b3*/ int { return i + w.i + b; }; 
  /*b3*/ int res = lambda(4);
```
The type of the **function object** which is constructed is of type "closure type" and its behavior is standardized by prescribing rules for
- how **captures** are mapped to members, 
- how the invocation is realized using an overloaded `operator()`, 
- which special member functions are available, and
- if conversion to a function pointer is possible (for non-capturing lambdas)

Note: the "closure type" is not directly accessible and compiler dependent; i.e., `auto` is used predominantly to deduce the type of function objects results from lambda expressions.

Considering the example from above and these rules just listed we could reformulate this
```pmans
  {
    int i{};
    Widget w{};
    auto /*b6*/ lambda = [/*b1*/ i, /*b2*/ &w](int /*b1*/ b) -> /*b3*/ int {/*b*/ return i + w.i + b; /*x*/}; 
    int res = /*b6*/ lambda(4);
  }
```
by using an inline class with 
- a custom constructor (1) which captures all required variables as members (1a)(1b),
- an overloaded `operator()` with the same *signature* as the "lambda" (2), and
- immediately instantiating the class (3):
```pmans
  {
    int i{};
    Widget w{};
    class /*f*/ ClosureType /*x*/ { 
    private:
      int /*b1*/ i;         // (1a) capture by-value
      Widget /*b2*/ &w;     // (1b) capture by-reference

    public:
      /*f*/ ClosureType /*x*/(int /*b1*/ i, Widget /*b2*/ &w) : /*b1*/ i(/*b1*/ i), /*b1*/ w(/*b1*/ w) {};               // (1)
      auto operator()(int /*b1*/ b) const -> /*b3*/ int { /*b*/ return i + w.i + b; /*x*/}   // (2)
    } /*b6*/ lambda(/*b1*/ i,/*b1*/ w);                                                  // (3)
    int res = /*b6*/ lambda(4);                                            // (4)
  }
```
The effect and syntax of an invocation (4) is then identical to the above lambda expression.

> How does the inline class look like when the captures are empty? Does this allow for further simplification?

> Can a lambda function be implicitly converted to a function pointer?
>```pmans
>  using FuncPtr = int (*)(int);
>  FuncPtr lambda = [](int a) -> int { return a + 2; }; // works?
>  int b = 5;
>  FuncPtr lambda = [b](int a) -> int { return a + b; }; // works?
>```

Let's look at non-capturing lambdas in a bit more detail.

## Non-capturing lambdas

Non-capturing lambdas can implicitly be converted to function pointers; capturing lambdas cannot.
To create some intuition for the reason behind this behavior, let's first briefly look at function pointers and conversion functions.
Equipped with this prerequisites we the again mimic the behavior using an inline class which converts to a function pointer implicitly (like a lambda). 

### Function pointers
A *function pointer* is a type holding a pointer to a function with a specific *signature*.
Some examples of non-member function pointers are:
```pmans
using FuncPtrRet/*b4*/ Void = /*b4*/ void (*)();
using FuncPtrRet/*b3*/ IntPass/*b3*/ Int/*b3*/ Int = /*b3*/ int (*)(/*b3*/ int, /*b3*/ int);
using FuncPtrRet/*b6*/ WidgetPass/*b9*/ WidgetRef = /*b6*/ Widget (*)(/*b7*/ Widget&);
```
Invocation of the above function pointers looks like this:
```pmans
// some functions with suitable signatures
void /*b*/ func_void /*x*/(){};
int /*b*/ func_int /*x*/(int a, int b) { return a + b; };
Widget /*b*/ func_widget /*x*/(Widget &&w) { return w; };
... 
  FuncPtrRetVoid fptr = /*b*/ func_void /*x*/;
  fptr();
  FuncPtrRetIntPassIntInt fptr2 = /*b*/ func_int /*x*/;
  fptr2(1, 2);
  FuncPtrRetWidgetPassWidgetRef fptr3 = /*b*/ func_widget /*x*/;
  fptr3(Widget{});
```
A useful application of function pointers is to use them as parameters:
```pmans
void /*f6*/ invoke(FuncPtrRetVoid callable) { callable(); }
...
  /*f6*/ invoke(func_void);
  /*f6*/ invoke(fptr);
```
### Conversion functions 
User-defined conversion functions allow to implement *implicit* and *explicit* conversions *form* a type *to* other types. 
> What is difference between explicit and implicit conversions?

A simple example of a conversion function looks like this:
```pmans
   struct Widget{
     int m;
     operator /*b5*/ int() { return /*b1*/ m; }; // (1) conversion from 'Widget' to 'int'
   }; 
   int d = Widget{3}; // implicit conversion used
   int dd = 3 + Widget{3} * 4; // implicit conversion used
``` 

### Lambda to function pointer conversion

For non-capturing lambdas, a conversion function can be used to allow a conversion to a function pointer. Let's look at a simple example again:
```pmans
    using /*b7*/ FuncPtr = /*f6*/ Widget (*)(/*f6*/ Widget &&);
    /*b7*/ FuncPtr fptr = /*b2*/ [](/*f6*/ Widget &&w) -> /*f6*/ Widget {
      ++(w.i);
      return w;
    };
    auto w = fptr(/*f6*/ Widget{});
```
The non-capturing lambda expression is implicitly converted to a function pointer. Also this conversion can be mimicked with an inline class:
```pmans
    class /*f*/ ClosureType /*x*/{
    private:
      static auto /*b6*/ invoke(Widget &&w) { // (1)
        ++(w.i);
        return w;
      }

    public:
      operator /*b7*/ FuncPtr() { return /*b6*/ invoke; } // (2)
      auto operator()(Widget &&w) {
        ++(w.i);
        return w;
      }
    };
    /*b7*/ FuncPtr fptr = /*f*/ ClosureType /*x*/();
    auto w = fptr(Widget{});
```
A static member function (1) is additionally implemented, which is then returned  when a conversion to a function pointer is performed (2). 

> When is this conversion to a function pointer useful?

> Why is this conversion not available if captures are present in the closure type?

## Special member functions
Up to now we did not provide user-defined special member functions of the inline classes we used to mimic lambda expressions (beside a custom constructor). 
The standard prescribes rules for the SMFs for function objects constructed using lambdas: default construction (1) is marked as `delete`d;
copy(2) and move(3) construction and destruction (6) are declared as `default`ed.
This results in an implicitly `deleted` copy assignment (4) and an undeclared move assignment operator (5).
Explicitly this looks like this for the introductory example above:
```pmans
int i{};
Widget w{};
class /*f*/ ClosureType /*x*/ { 
private:
  int /*b1*/ i;
  Widget /*b2*/ &w;

public:

  /*f*/ ClosureType /*x*/() = /*b7*/ delete; // (1)
  /*f*/ ClosureType /*x*/(const /*f*/ ClosureType /*x*/ &) = /*b7*/ default; // (2)
  /*f*/ ClosureType /*x*/(/*f*/ ClosureType /*x*/ &&) = /*b7*/ default; // (3)
  /*f*/ ClosureType /*x*/ &operator=(const /*f*/ ClosureType /*x*/ &) = /*b7*/ delete; // (4) 
  // (5) ClosureType &operator=(ClosureType&&)
  ~/*f*/ ClosureType /*x*/() = /*b7*/ default; // (6)

  /*f*/ ClosureType /*x*/(int /*b1*/ i, Widget /*b2*/ &w) : /*b1*/ i(/*b1*/ i), /*b1*/ w(/*b1*/ w) {};      
  auto operator()(int /*b1*/ b) const -> /*b3*/ int { /*b*/ return i + w.i + b; /*x*/}
} /*b6*/ lambda(i,w); 
int res = /*b6*/ lambda(4);
```
Exploring the effect of these rules for the SMFs can look like this:
```pmans
    const int i{2};
    Widget w{3};
    auto lambda = [&i, &w](int b) { return i + w.i + b; };
    auto lambda_copy = lambda;           // copy ctor
    auto lambda_move = std::move(lambda);// move ctor
    lambda_copy = lambda;                // error: copy assignment
    lambda_copy = std::move(lambda);     // error: move->copy assignment
```
## Evaluation time vs. execution time
For capturing lambdas, it is important to differentiate the time of *evaluation* of a lambda expression and the subsequent *invocations/executions* of the constructed function object:
the function object is constructed at *evaluation time* (1) of the lambda expression. At invocation time (2a)(2b) changes to variables captured by reference `&w` are reflected while the internal state to variables captured by value `x` is effective.
 ```pmans
int x = 5; 
Widget w{7}; 
auto lambda = [x,&w](){ ++x; return w.m + x;} // (1)
x = 100;
w.m = 100;
auto res1 = lambda(); // (2a)
x = 0;
w.m = 0;
auto res2 = lambda(); // (2b)
```
> What value do `res1` and `res2` have?

## Capture options
Above, up to now we only cherry-picked captures explicitly by-reference or by-value for local variables in the outer scope.

### Capture defaults/wildcards
To obtain a default capture by-value for "everything required" in the body of the lambda, a `=` as first item in the capture list can be used:
```pmans
    double g;
    const int i{2};
    Widget w{3};
    // default capture by-value    
    auto lambda = [/*b1*/ =](int b) { return g + w.i + b + i; }; 
```
Similar for a default-capture by lvalue reference, `&` as first item in the capture list can be used:
```pmans
    // default capture by-reference    
    auto lambda = [/*b1*/ &](int b) { return g + w.i + b + i; }; 
```
The default can be mixed with explicit exceptions from the default:
 ```pmans
    // explicit capture by-value for i,g
    auto lambda = [/*b1*/ &, i, g](int b) { return g + w.i + b + i; }; 
    // explicit capture by-reference for i,g    
    auto lambda = [/*b1*/ =, &i, &g](int b) { return g + w.i + b + i; };    
```
Further, names of variables can be provided explicitly:
  ```pmans
    // explicit names for i and g
    auto lambda = [/*b1*/ =, ref_i=&i, ref_g=g](int b) { return ref_g + w.i + b + ref_i; }; 
```
Moving-constructing an object into function object is achieved using this syntax:
  ```pmans
    // explicit names for i and g
    auto lambda = [/*b1*/ =, moved_w=/*b*/ std::move /*x*/(w)](int b) { return g + moved_w.i + b + i; }; 
```

### Enclosing object 
> What happens if a lambda is defined in a scope with access to a `this` pointer and implicitly access the object pointed-to by `this`?

```pmans
    struct Widget {
      int /*b1*/ m;
      auto member() {
        return [=]() { return /*b1*/ m; };
        return [=]() { return /*b7*/ this->m; }; // equivalent
      }
    };
    auto lambda = Widget{9}.member();
    std::cout << lambda() << std::endl;
```
To explicitly control type of capture for the enclosing object `this` can be captured by-copy (1) or by reference (2):
```pmans
    struct Widget {
      int m;
      auto member() {
        return [/*b4*/ this]() { return m; }; // (1) by-reference 
        return [/*b5*/ *this]() { return m; }; // (1) by-copy
      }
    };
```

## Generic lambdas
If one or more of the parameters use `auto` to specific the type the lambda expression is called *generic lambda*:
```pmans
    const int i{2};
    Widget w{3};
    // generic lambda   
    auto lambda = [=](/*b4*/ auto b, double g) { return g + w.i + b + i; };
    auto lambda = [=](/*b6*/ auto &b, double g) { return g + w.i + b + i; }; 
    auto lambda = [=](/*b7*/ auto &&b, double g) { return g + w.i + b + i; };     
```
Again, to mimic this effect with a class (not an inline class anymore as templates are not allowed inline) the `operator()` is transformed into a templated member function:
```pmans
class ClosureType { 
private:
  int i;    // (1a) capture by-value (default)
  Widget w; // (1b) capture by-value (default)

public:
...
  ClosureType(int i, Widget &w) : i(i), w(w){}; // (1)
  /*f9*/ template <typename /*b4*/ AUTO> auto operator()(/*b4*/ AUTO b, double g) { ... }
  /*f9*/ template <typename /*b4*/ AUTO> auto operator()(/*b5*/ AUTO& b, double g) { ... }
  /*f9*/ template <typename /*b4*/ AUTO> auto operator()(/*b6*/ AUTO&& b, double g) { ... } 
... 
};
```

## Examples
Finally lets look at some examples

## Custom comparator
```pmans
 struct Widget {
    int m;
  };
  std::vector<Widget> vec{};
  vec.push_back(Widget{10});
  vec.push_back(Widget{5});
  vec.push_back(Widget{4});


  int threshold = 2;
  auto comp1 = [threshold](const Widget &a, const Widget &b) {
    return std::abs(a.m - b.m) > threshold ? a.m < b.m : false;
  };
  auto comp2 = [](const Widget &a, const Widget &b) {
    return a.m < b.m;
  };  

  std::sort(vec.begin(), vec.end(), comp1);
  for (auto&& item : vec) {
    std::cout << item.m << " ";
  }
```

## Custom predicate
```pmans
  struct Widget {
    int m;
  };
  std::vector<Widget> vec{};
  vec.push_back(Widget{10});
  vec.push_back(Widget{5});
  vec.push_back(Widget{4});

  int lower = 2;
  int upper = 5;
  auto lambda = [&t1 = lower, &t2 = upper](const Widget &a) {
    return a.m > t1 && a.m < t2;
  };

  {
    auto count = std::count_if(vec.begin(), vec.end(), lambda);
    std::cout << count << std::endl;
  }
  ++upper;
  {
    auto count = std::count_if(vec.begin(), vec.end(), lambda);
    std::cout << count << std::endl;
  }
```

## Dangling capture
```pmans
int main() {

  auto lambda = []() {
    int x{};
    return [&x]() { return x + 5; };
  };
  auto res = lambda();
}
```

## Links

- Lambda expression: https://en.cppreference.com/w/cpp/language/lambda
- Cppcon2020, Lambda expressions: https://www.youtube.com/watch?v=ZIPNFcw6V9o
- Andreas Fertig, Lambdas https://www.youtube.com/watch?v=M0qy6RiMU90
