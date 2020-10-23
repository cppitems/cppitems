9 // item status
# Exercise EX1.1

This exercise is one of three parts of EX1. The other two parts (EX1.2 and EX1.3) will be rolled out separately. See also https://cppitems.github.io/#/item/000 for an overview and deadlines. The submission deadline for EX1 (all three parts) is **Mo 09.11.2020, 4pm**.
The sources related to EX1.1 are available at https://github.com/cppitems/cppitems/tree/master/items/005 .

## Task description

This exercise deals with a `Widget` type which looks like this:
```pmans
struct /*f*/ Widget /*x*/{
  std::vector<double> /*f3*/ vec;
};
```
`Widget` has a single non-reference member `vec` which is of type `std::vector<double>`.

Your task is to complete the implementaton of two overloaded versions of an initialization function (in `benchmarking_initializers.cpp`) which initialize a `Widget` using references to an already existing `std::vector<double>` object. 
You should not change any code beside the function body of these two functions (which already exist, but are 'empty').

### Overload 1: rvalue reference

The first overload takes a single argument of type 'rvalue reference to `std::vector<double>`' and returns a `Widget` by value:
```pmans
Widget /*f4*/ init(/*b*/ std::vector<double> && /*x*/rref) {
  ...
};
```
You have to implement the function and make 'best use' of the rvalue reference, i.e., it should be moved into the returned `Widget` object for performance reasons.

### Overload 2: lvalue reference

The second overload takes a single argument of type 'lvalue reference to const `std::vector<double>`' and returns a `Widget` by value:
```pmans
Widget /*f4*/ init(/*b*/ const std::vector<double> & /*x*/lref) {
  ...
};
```
Again, it is your task to implement the function. As now an lvalue reference is passed, the `std::vector<double>` cannot be 'moved into' the `Widget` but instead has to be copied.  

## Benchmarking
The `benchmarking_initializers.cpp` also benchmarks the performance of the initalizations (once you implemented them) by measuring the run time of
```pmans
    Vector vec(size, std::rand());
    Widget w = init(std::move(vec)); // resolved to rvalue overload
```
and
```pmans
    Vector vec(size, std::rand());
    Widget w = init(vec);            // resolved to lvalue overload
```
and prints the respective run times to the console.

## Reference benchmark
As reference, a benchmark is provided in `benchmarking_constructors.cpp`. It analogously measures the run time of
```pmans
    Widget var = Widget(size, std::rand());
    Widget w = /*f*/ std::move /*x*/(var) ; // invoking implicit move construction
```
and
```pmans
    Widget var = Widget(size, std::rand());
    Widget w = var; // invoking implicit copy construction
```    
for the implicitly defined copy/move constructors. They are explicitly defaulted to reveal the similarity of their prototypes (i.e., overloading using different reference types) to your initialization functions: 
```pmans
struct Widget {
  std::vector<double> vec;
  Widget(size_t size, double init) : vec(size, init) {}
  Widget(/*b*/ const Widget & /*x*/) = default; // implicit copy constructor
  Widget(/*b*/ Widget && /*x*/) = default;      // implicit move constructor
};
```