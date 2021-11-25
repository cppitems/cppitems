1 // item status
# Lambda expressions
We have already used "lambdas" in previous items. 
Let's now look at *lambda expressions* in detail.

A *lambda expression* constructs a *function object*, i.e., a class with an overloaded `operator()`, which is able to capture variables which are valid at the source location where the lambda expression is evaluated. 
This *function object* is of unnamed type and is ready to be invoked "like a function".
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
  int res = /*f6*/ lambda(4); // execution/invocation of function object
```
> What is the maximum length for the code block in the body of a lambda?  
> - there is no restriction


The **capture clause** lists which variables of the "outside scope" are captured for a use inside the **body**:
```pmans
  int /*b1*/ i{};
  Widget /*b1*/ w{}; 
  auto lambda = [/*b1*/ i, /*b2*/ &w] (int b) -> int { 
    i = 10;  // no effect on outer scope
    w.i = 10; // effect on 'original' w: as it was captured by-reference     
      return /*b1*/ i + /*b1*/ w.i + b;
  }; 
  ...
```
> Can the variable names be altered during capturing?
>```pmans
>  int b = 5;
>  FuncPtr lambda = [myb = b](int a) -> int { return a + myb; }; // rename 'b' ?
> // 
>```

```pmans
// classic
typename Widget::nested_type func(int arg) {return Widget{}.m; };
// trailing return type
template<typename T>
auto func(T arg) -> decltype(arg) { return arg;};
decltype(arg) func(T arg) { return arg;} // not working (left to right symbok checking)
```

> What is the effect of using `&w` compared to `w` in the capture clause?
> - capture "by-reference" instead of "by-copy"


The **parameter list** lists the parameter which are required when invoking the function object; they are also available in the body (as for any normal function):
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
      //int /*b1*/ i;         // (1a) capture by-value
      //Widget /*b2*/ &w;     // (1b) capture by-reference

    public:
      /*f*/ ClosureType /*x*/(int /*b1*/ i, Widget /*b2*/ &w) : /*b1*/ i(/*b1*/ i), /*b1*/ w(/*b1*/ w) {};               // (1)
      auto operator()(int /*b1*/ b) const -> /*b3*/ int { /*b*/ return i + w.i + b; /*x*/}   // (2)
    } /*b6*/ lambda(/*b1*/ i,/*b1*/ w);                                                  // (3)
    ClosureType lambda(/*b1*/ i,/*b1*/ w); // ok also
    int res = /*b6*/ lambda(4);                                            // (4)
  }
```
The effect and syntax of an invocation (4) is then identical to the above lambda expression.

> How does the inline class look like when the captures are empty? Does this allow for further simplification?
> - no data members -> stateless function object


> Can a lambda function be implicitly converted to a function pointer?
>```pmans
>  using FuncPtr = int (*)(int);
>  FuncPtr lambda = [](int a) -> int { return a + 2; }; // works? yes, no members! behaves like a free function
>  int b = 5;
>  FuncPtr lambda = [b](int a) -> int { return a + b; }; // works? no, has members, function needs access to this state
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
> - a conversion is implicit if the type which is the target of the conversion is not explicitly stated in the code, but the required target type is deduced from the semantic embedding
> - an explicit conversion does prescribe the target type of the conversion independent of the semantic embedding
> - the specifier `explicit` can be used to disable the participation of a constructor or conversion function in implicit conversions 

A simple example of a conversion function looks like this:
```pmans
   struct Widget{
     int m;
     operator /*b5*/ int() { return /*b1*/ m; }; // (1) conversion function from 'Widget' to 'int'
   }; 
   int d = Widget{3}; // implicit conversion used
   int dd = 3 + Widget{3} * 4; // implicit conversion used
``` 

A second example where a conversion function (2) and a constructor (1) participating in conversions looks like this:
```pmans
struct Widget1 {
  int m;
};

struct Widget2 {
  int m;
  Widget2() : m(){};
  // (1) converting constructor from 'Widget1' to 'Widget2'
  /*b9*/ explicit Widget2(const Widget1 &w) : m(w.m){};
  // (2) conversion function from 'Widget2' to 'Widget1'
  /*b9*/ explicit operator Widget1() { return Widget1{m}; };
};

int main() {
  Widget1 w1;
  Widget2 w2;
  w2 = w1; // implicit conversion using (1): error if (1) is specified '/*f9*/ explicit'
  w2 = static_cast<Widget2>(w1);  // explicit conversion using (1)
  w1 = w2; // implicit conversion function (2): error if (2) is specified '/*f9*/ explicit'
  w1 = static_cast<Widget1>(w2);  // explicit conversion using (2)
}
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
> - this conversion allows non-capturing lambdas to be used in situations requiring function pointers 
> - this conversion provides a convenient way to define functions in a local scope


> Why is this conversion not available if captures are present in the closure type?
> - captures effectively create a stateful local function object; invoking the lambda requires access to this object
> - a function does not have this access/object

## Special member functions
Up to now we did not provide user-defined special member functions of the inline classes we used to mimic lambda expressions (beside a custom constructor). 
The standard prescribes rules for the SMFs for function objects constructed using lambdas: 
- default construction (1) is not available
- copy(2) and move(3) construction are declared as `default`ed
- copy assignment (4) is declared as `delete`d, (move assignment not declared)
- the destructor (6) is implicitly declared

Again mimicking these rule within the inline class this looks like this for the introductory example above:
```pmans
int i{};
Widget w{};
class /*f*/ ClosureType /*x*/ { 
private:
  int /*b1*/ i;
  Widget /*b2*/ &w;

public:

  /*f*/ ClosureType /*x*/() = /*b7*/ delete; // (1) not defined
  /*f*/ ClosureType /*x*/(const /*f*/ ClosureType /*x*/ &) = /*b7*/ default; // (2)
  /*f*/ ClosureType /*x*/(/*f*/ ClosureType /*x*/ &&) = /*b7*/ default; // (3)
  /*f*/ ClosureType /*x*/ &operator=(const /*f*/ ClosureType /*x*/ &) = /*b7*/ delete; // (4) 
  // /*f*/ ClosureType /*x*/ &operator=(/*f*/ ClosureType /*x*/&&); // (5) not declared
  ~/*f*/ ClosureType /*x*/() = /*b7*/ default; // (6) implicit
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
    lambda_copy = std::move(lambda);     // error: move (not available due to reference member) -> fallback copy assignment
```
## Evaluation time vs. execution time
For capturing lambdas, it is important to differentiate the time of *evaluation* of a lambda expression and the subsequent *invocations/executions* of the constructed function object:
the function object is constructed at *evaluation time* (1) of the lambda expression. At invocation time (2a)(2b) changes to variables captured by reference `&w` are reflected while the internal state of variables captured by-value (`x`) is effective. In (1) additionally the keyword `mutable` is required to allow the invocation to change the internal state (i.e., `++x`) of the function object.
 ```pmans
int x = 5; 
Widget w{7}; 

auto lambda = [x,&w]() /*b7*/ mutable -> auto { ++x; return (w.m + x);} // (1) 
// lambda.x == 5;
// w just alias -> reference member
x = 100; // change x to 100 in this scope
w.m = 100; // influences lambda -> lamnda.w.m == 100
auto res1 = lambda(); // (2a)
// 1. ++(lambda.x) -> lambda.x == 6
std::cout << res1 << std::endl; // 106
x = 0; // reset in this scope (no effect on lambda)
w.m = 0; // effect on lambda 
auto res2 = lambda(); // (2b) 
// 1. inc lambda.x
// 2. return x == 7 
// -> 7
```
> What are the values of `res1` and `res2`?
<!-- 
> - `res1 = w.m + x;` where `x=5+1` and `w.m` = 7 
> - `res2 = w.m + x;` where `x=6+1` and `w.m` = 0 
-->
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
// [=] -> copy by-value (but only what is required for the body)     
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
    auto lambda = [/*b1*/ =, i=&i, ref_g=g](int b) { return i + ref_g + w.i + b + i; }; 
```
Moving-constructing an object into function object is achieved using this syntax:
  ```pmans
    // explicit names for i and g
    auto lambda = [/*b1*/ =, moved_w=/*b*/ std::move /*x*/(w)](int b) { return g + moved_w.i + b + i; }; 
```

### Enclosing object 
> What happens if a lambda is defined in a scope with access to a `this` pointer and implicitly accesses the object pointed-to by `this`?
> - if `this` is captured implicitly  (through  one of the two default capture modes `[=]` or `[&]`) the object referred to by this is captured by reference (see snippet below)


```pmans
  struct Widget {
    int /*b1*/ m;
    auto member() {
      this->m;        
      m;
      return [/*b1*/ &]() { return /*b7*/ this->m; }; // (1) 'this' is captured by reference
      return [/*b1*/ =]() {return /*b1*/ m; };       // (2) 'this' is still captured by reference
    }
  };
  auto func = []() {return Widget{9}.member(); };  // temporary 'Widget{9}' is destroyed after this line
  auto lambda = func();                            // lambda hols a dangling reference to the temporary
  std::cout << lambda() << std::endl;              // printing undefined value
```
To explicitly control type of capture for the enclosing object `this` can be captured by copy (1) or by reference (2):
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
This snippet uses lambdas to setup custom comparators which are the used to sort a vector of `Widgets`:
```pmans
 struct Widget {
    int m;
  };
  std::vector<Widget> vec{};
  vec.push_back(Widget{10});
  vec.push_back(Widget{5});
  vec.push_back(Widget{4});


  int threshold = 2;
  // this comparator uses a local variable(s) to configure a threshold for the sorting
  // this could not be easily achieved with a function pointer
  auto /*b5*/ comp1 = [threshold](const Widget &a, const Widget &b) {
    return std::abs(a.m - b.m) > threshold ? a.m < b.m : false;
  };
  auto comp2 = [](const Widget &a, const Widget &b) {
    return a.m < b.m;
  };  

  // bool = comp1(item, item);
  // if true: a is smaller
  // if false: b is not smaller
  std::sort(vec.begin(), vec.end(), /*b5*/ comp1); 

  for (auto&& item : vec) {
    std::cout << item.m << " ";
  }
```

## Custom predicate
This snippet uses a lambda to setup a predicate which is used to count the `Widget`s in a vector, which have the property tested by the predicate:
```pmans
  struct Widget {
    int m;
  };
  std::vector<Widget> vec{};
  vec.push_back(Widget{10});
  vec.push_back(Widget{5});
  vec.push_back(Widget{4});

  int /*b5*/ lower = 2;
  int /*b5*/ upper = 5;
  // 
  auto /*f6*/ lambda = [&t1 = /*b5*/ lower, &t2 = /*b5*/ upper](const Widget &a) {
    return a.m > t1 && a.m < t2;
  };

  { // count elements using predicate
    auto count = std::count_if(vec.begin(), vec.end(), /*f6*/ lambda);
    std::cout << count << std::endl;
  }
  ++/*b5*/ upper; // increasing upper threshold of predicate
  { // count elements using predicate
    auto count = std::count_if(vec.begin(), vec.end(), /*f6*/ lambda);
    std::cout << count << std::endl;
  }
```

## Dangling capture
This snippet shows an example where a lambda captures a variable of the local scope by reference. This leads to a dangling reference if the lambda is invoked after this variable has gone out of scope:
```pmans
int main() {

  auto lambda = []() {
    int /*b1*/ x{};
    return [&/*b1*/ x]() { return /*b1*/ x + 5; }; // capturing  local '/*f1*/ x' by reference
  };
  auto res = lambda(); 
  // lambda holds dangling reference to the '/*f1*/ x' in the body of the lambda (was released after the operator() call completes)
}
```


## Additional example

```pmans
// std::transform signature
template< class InputIt,
          class OutputIt,
          class UnaryOperation >
OutputIt transform( InputIt first1,
                    InputIt last1,
                    OutputIt d_first,
                    UnaryOperation unary_op );
```
Let's look at different example how to use the overloads of `tan` in `cmath` inside the unary operation:
```pmans

// exemplaric overloads
double tan(double in) {return in;}; 
float tan(float in) {return in;}; 

int main(){
  using T  = double;    
  std::vector<T> v(10, 12.);
  using return_type = T;
  using argument_type = T;

  std::transform<typename v::iterator, typename v::iterator,  double (*)(double) >(  v.begin(),v.end(), v.begin(), 
                        tan);    

  // old way

  using FPtr = return_type (*) (argument_type1);
  std::transform( v.begin(),
                  v.end(), 
                  v.begin(), 
                  //static_cast<typename v::value_type (*)(argument_type)>(tan) 
                  static_cast<return_type (*)(argument_type)>(tan)              
                  );
  // solves the problem of seleting the right overload most elegantly
  std::transform( v.begin(),
                  v.end(), 
                  v.begin(), 
                  [](const auto& a){ return tan(a); }                
                  );                  
}
```

## Links

- Lambda expression: https://en.cppreference.com/w/cpp/language/lambda
- Cppcon2020, Lambda expressions: https://www.youtube.com/watch?v=ZIPNFcw6V9o
- Andreas Fertig, Lambdas https://www.youtube.com/watch?v=M0qy6RiMU90
