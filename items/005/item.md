1 // item status
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

Your task is to complete the implementaton of two overloaded versions of an initialization function `init(...)`and a third non-overloaded function `init_byvalue(...)` in `benchmarking_initializers.cpp`.
All three functions initialize a `Widget` using an existing `std::vector<double>` object which is passed as argument.
You should not change any code beside the function bodies of these three functions (which already exist, but are 'empty').

Finally, you should benchmark your implementations and prepare yourself for a discussion on the obtained runtime differences when benchmarking your implementations using the benchmark setup which is also provided in `benchmarking_initializers.cpp`.

### Overload 1: rvalue reference

The first overload takes a single argument of type 'rvalue reference to `std::vector<double>`' and returns a `Widget` by value:
```pmans
Widget /*f4*/ init(/*b*/ std::vector<double> && /*x*/rref) {
  ...
};
```
You have to implement the function and make 'best use' of the rvalue reference
The argument is of type rvalue reference and only binds to movable objects, which means it can be 'moved into' the returned `Widget` object for performance reasons.

### Overload 2: lvalue reference

The second overload takes a single argument of type 'lvalue reference to const `std::vector<double>`' and returns a `Widget` by value:
```pmans
Widget /*f4*/ init(/*b*/ const std::vector<double> & /*x*/lref) {
  ...
};
```
Again, it is your task to implement the function. As now the argument if of type lvalue reference, the `std::vector<double>` cannot be 'moved into' the `Widget` but instead has to be copied.  

### Option 3: by-value

The third function `init_byvalue(...)` takes a single non-reference argument of type `std::vector<double>` and returns a `Widget` by value:
```pmans
Widget /*f*/ init_byvalue /*x*/(/*b*/ const std::vector<double>  /*x*/vec) {
  ...
};
```
Again, it is your task to implement the function and to make 'best use' of the local copy of the `std::vector<double>` which is available by means of the function argument.

## Benchmarking
The `benchmarking_initializers.cpp` benchmarks the performance of all three  initalization functions (once you implemented them) by measuring the run time of
```pmans
    Vector vec(size, std::rand());
    Widget w = init(std::move(vec)); // rvalue overload of init(...)
```
and
```pmans
    Vector vec(size, std::rand());
    Widget w = init(vec);            // lvalue overload of init(...)
```
for the overloads and
```pmans
    Vector vec(size, std::rand());
    Widget w = init_byvalue(vec);     // uses init_byvalue(...)
```
for the third function.

The respective runtimes are printed to the console. 