1 // item status
# Exercise EX2.1

This exercise is part **one** of three parts of EX2. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX2 (all three parts) is **Mo 30.11.2020, 4pm**. The sources related to EX2.1 are available at [Item 011](https://github.com/cppitems/cppitems/tree/master/items/011).

## Task description

In this exercise it is your task to adopt the implementation of the functions in `algorithms.h` ([link](https://raw.githubusercontent.com/cppitems/cppitems/master/items/011/algorithms.h)):
These functions are function templates with a single *type template parameter* `C` and implement algorithms tailored to a "container type" `C` which supports access to elements using the `operator[]` and has a `size()` member (aka a `std::vector` in our case).
For example, the `print` function which prints the content of the container to `std::cout` looks like this:
```pmans
/*f*/ template <typename /*b1*/ C> /*x*/ 
auto /*f5*/ print(const std::string &text, const /*b1*/ C &ctnr) {
  std::cout << std::setw(23) << text << ": ";
  /*b5*/ for (std::size_t i = 0; i < ctnr.size(); ++i/*b1*/ ) {
    std::cout << ctnr[i] << "|";
  }
  std::cout << std::endl;
};
```
More specifically, you should change the implementation of **all functions** in `algorithms.h` wherever a "classic" `for`-loop is present.
Instead of the `for`-loop you should make best use of the *iterator-based* algorithms (function templates) from `#include <algorithm>` without changing the original logic of the implementation.
For example, the implementation of the `print` function can the look like this:
```pmans
/*f*/ template <typename /*b1*/ C> /*x*/ 
auto /*f5*/ print(const std::string &text, const /*b1*/ C &ctnr) {
  auto /*f4*/ func = [](const typename /*b1*/ C::value_type &item) {
    std::cout << item << "|";
  }; // function object from lambda expression
  std::cout << std::setw(23) << text << ": ";
  /*b*/ std::for_each( /*x*/ctnr.begin(), ctnr. end(), /*f4*/ func/*b1*/ ); // for_each from <algorithm>
  std::cout << std::endl;
};
```
The function now uses the `std::for_each` function template (as `std::vector` provides sufficient support for *iterators*) and uses a *lambda expression* to create a *function object* `func` to implement the function to be applied to each element in the container.

Another example of the adoption you should perform is the `populate_with_value` function template which originally looks like this:
```pmans
/*f*/ template <typename /*b1*/ C> /*x*/ 
auto populate_with_value(/*b1*/ C &ctnr, typename /*b1*/ C::value_type value) {
  /*b5*/ for (std::size_t i = 0; i < ctnr.size(); ++i/*b1*/ ) {
    ctnr[i] = value;
  }
};
```
When using `std::fill` to remove the explicit `for`-loop it can look like this:
```pmans
/*f*/ template <typename /*b1*/ C> /*x*/ 
auto populate_with_value(/*b1*/ C &ctnr, typename /*b1*/ C::value_type value) {
  /*b*/ std::fill( /*x*/ctnr.begin(), ctnr.end(), value/*b1*/ );
};
```

For an overview and documentation of all algorithms available in the algorithms library of the C++ standard library (`#include <algorithm>`) see [https://en.cppreference.com/w/cpp/algorithm]([https://en.cppreference.com/w/cpp/algorithm).

Note: after you finished your modifications in `algorithms.h`, no explicit `for`-loops should be present.

Prepare yourself for a discussion of your implementation and try to identify advantages and disadvantages of using "algorithms form the stdlib" vs. "classic `for`-loops".

## A mini introduction on using iterators with `std::vector`
We did not discuss *iterators* in detail in the lectures yet. Nevertheless, you have to utilize algorithms from `<algorithm>` for the tasks required in this exercise (`std::vector`); this is possible without deep knowledge of the iterator concept. 
To provide a minimal guideline, `vector_iterators.cpp` ([link](https://raw.githubusercontent.com/cppitems/cppitems/master/items/011/vector_iterators.cpp)) contains some examples using iterators and a `std::vector<double>`, which cover the use cases in this exercise.

Further resources are:

- std::begin(...): https://en.cppreference.com/w/cpp/iterator/begin
- std::end(...): https://en.cppreference.com/w/cpp/iterator/end
- Iterator library: https://en.cppreference.com/w/cpp/iterator
- Algorithm Library: https://en.cppreference.com/w/cpp/algorithm
- std::vector: https://en.cppreference.com/w/cpp/container/vector

## Testing
In `test_algorithms.cpp`  ([link](https://raw.githubusercontent.com/cppitems/cppitems/master/items/011/test_algorithms.cpp)) provides small tests/logging for each of the algorithms in `algorithms.h`. We will also use these tests to test your submission. 
The tests apply each of the algorithms in `algorithms.h` with a `std::vector<int>` as first argument.
For example the test for the `populate_with_value` function template is called like this:
```pmans
  ...
  /*f*/ populate_with_value /*x*/(/*b3*/ vec, value_type{1});
  print("populate_with_value", vec); // logging the result
```

## Submission
Finally, you should submit EX2 (all three parts) using a single repository `ex2`.
Therefore you repo should look like this after EX2.1:
```
test_algorithms.cpp
algorithms.h
CMakeLists.txt
```
Further you should add all relevant targets in the `CMakeLists.txt` which should look similar to this after EX2.1:
```
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
project(ex2 LANGUAGES CXX)
add_executable(test_algorithms test_algorithms.cpp)
```
