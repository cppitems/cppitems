9 // item status
# Exercise EX1.1

This exercise is one of three parts of EX1; submission deadline for EX1 is **Mo 09.11.2020, 4pm**. See also https://cppitems.github.io/#/item/000 for an overview of dates and deadlines.

The source related to this exercise are available at https://github.com/cppitems/cppitems/tree/master/items/005 .

This exercise deals with a `Widget` type which looks like this:
```pmans
struct /*f*/ Widget /*x*/{
  std::vector<double> /*f3*/ vec;
};
```
`Widget` has a single non-reference member `vec` which is of type `std::vector<double>`.

Your task is to complete the implementaton of two overloaded versions of an initialization function (in `benchmarking_initializers.cpp`) which initialize a `Widget` using references to an already existing `std::vector<double>` object. 
Otherwise you should not change anything beside the function body of these functions (which already exist, but are 'empty').

## Rvalue reference

The first overload takes a single argument of type 'rvalue reference to `std::vector<double>`' and returns a `Widget` by value:
```pmans
Widget /*f4*/ init(/*b*/ std::vector<double> && /*x*/rref) {
  ...
};
```
You have to implement the function and make 'best use' of the rvalue reference, i.e., it should be moved into the returned `Widget` object for performance reasons.

## Lvalue reference

The second overload takes a single argument of type 'lvalue reference to const `std::vector<double>`' and returns a `Widget` by value:
```pmans
Widget /*f4*/ init(/*b*/ const std::vector<double> & /*x*/lref) {
  ...
};
```
Again, it is your task to implement the function. As now an lvalue reference is passed, the `std::vector<double>` cannot be 'moved into' the `Widget` but instead has to be copied.  

## Benchmarking
The `benchmarking_initializers.cpp` benchmarks the performance of the initalizations (once you implemented them) by averaging the run time of
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
  Widget(size_t size, std::vector<double>::value_type init) : vec(size, init) {}
  Widget(/*b*/ const Widget & /*x*/) = default; // implicit copy constructor
  Widget(/*b*/ Widget && /*x*/) = default;      // implicit move constructor
};
```