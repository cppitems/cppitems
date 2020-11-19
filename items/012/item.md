1 // item status
# Exercise EX2.2

This exercise is part **two** of three parts of EX2. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX2 (all three parts) is **Mo 30.11.2020, 4pm**. The sources related to EX2.2 are available at [Item 012](https://github.com/cppitems/cppitems/tree/master/items/012).

## Task 1

The first task in this exercise is to implement the calculation of the *arithmetic mean* and *standard deviation* of the collection of elements in a container. The implementation is required inside the `Distribution` class (`Distribution.hpp`) which looks like this:
```pmans
template <typename T> struct Distribution {
  using value_type = T;
  T mean; 
  T stddev; 

  // construct from given properties
  Distribution(T mean, T stddev) : mean(mean), stddev(stddev){};

  // construct from a dataset (std::vector)
  Distribution(std::vector<T> &dataset) : mean(0), stddev(0) {
    // (Task 1) missing implementation which calculates mean and stddev of the dataset
  }

  // (Task 2) missing functionality to construct a Distribution from other containers
};
```
The implementation at (1) is at least required to work for these for types `value_types` of the container:
- `double`
- `float`
- `long`
- `SpaceVector<double>` (from `SpaceVector.hpp`)

**Hint**: `SpaceVector` already supports the required operators to calculate the mean and standard deviation including also `sqrt`.

## Task 2
The second task in this exercise is to extend the implementation of the `Distribution` class to support a construction from different container types. 
At least the following container types should be supported: `std::vector`, `std::list`, `std::deque` .
More specifically, the requirement is that the syntax as used in `test_distribution.cpp` is valid for your implementation.
An example of how this syntax looks like in `test_distribution.cpp` is this:
```pmans
/* file: test_distribution.cpp */
  ...
  using CTNR = std::vector<T>;
  CTNR /*f4*/ ctnr;
  // constructing type /*f*/ Distribution /*x*/ without providing explicit template arguments
  /*b*/ Distribution /*x*/ final_distr(/*f4*/ ctnr); 
```

Note: After you completed your implementation, the project should compile without errors.

Note: You are encouraged to adopt the existing part of the implementation of `Distribution `.

**Hint**: You should use templates to avoid code duplication and use the common interfaces of these different containers in your implementation. In this way you could solve **Task 1** and **Task 2** with just one implementation.

## Testing
The file `test_distribution.cpp` contains several use cases of of the `Distribution` class covering the requirements for supported container types and `value_types` mentioned above. We will use this file to test your submission.

Note: You should not change any code in `test_distribution.cpp`; we will use an unmodified version to test your submission.

**Hint**: When you start working on this exercise, the project will not compile without errors. You can use the compiler argument "-fmax-errors=1" (g++) to stop compilation after the first error to prevent "meter-long" error messages w.r.t. failed template instantiations. The provided CMakeLists.txt contains these limits for the GNU (g++) and the Clang compilers. If you use a different compiler, you are encouraged to set the error limit yourself.

## Submission
Finally, you should submit EX2 (all three parts) using a single repository `ex2`.
Therefore you repo should look like this after EX2.2:
```
test_algorithms.cpp
algorithms.h
test_distribution.cpp
Distribution.hpp
SpaceVector.hpp
CMakeLists.txt
```
Further you should add all relevant targets in the `CMakeLists.txt` which should look similar to this after EX2.2:
```
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
project(ex2 LANGUAGES CXX)
add_executable(test_algorithms test_algorithms.cpp) # this line is for EX2.1
add_executable(test_distribution test_distribution.cpp) # this line is for EX2.2
```

