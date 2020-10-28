1 // item status
# Exercise EX1.2
This exercise is **part two** of three parts of EX1. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX1 (all three parts) is **Mo 09.11.2020, 4pm**. The sources related to EX1.2 are available at [Item 007](https://github.com/cppitems/cppitems/tree/master/items/007).

## Task description

This exercise deals with a `Widget` type which looks like this:
```pmans
struct /*b*/ Widget /*x*/ {
  /*f*/ Vector<double> /*x*/ vec;  // you will implement parts of 'Vector' 
  Widget(size_t count, const /*f*/ Vector<double> /*x*/::value_type& value) 
  : vec(count, value) {}
}; 
```
`Widget` has a single non-reference member `vec` which is of type `Vector<double>` (i.e., the specialization of a `Vector` template-class for `double`).

A defective implementation of the `Vector` template class is provided in `vector.hpp`.
It is your task to change the *defaulted* implementations of (1) the copy constructor, (2) the move constructor, and (3) the destructor of `Vector` to match the demands for this resource owning type. You should only change code of these three special member functions.
The defective implementation of `Vector` you start with looks like this:
```pmans
template <class T> class /*f6*/ Vector {
public:
  using value_type = T;
  using size_type = std::size_t;

private:
  size_type dataSize = 0;
  value_type *dataPointer = nullptr;

public:
  /*f6*/ Vector() {}
  /*f6*/ Vector(size_type count, const T &value)
      : dataSize(count), dataPointer(new T[count]) {
    for (std::size_t i = 0; i < count; ++i) {
      dataPointer[i] = value;
    }
  }
  /*f6*/ Vector(const Vector &vec) = /*b*/ default /*x*/; // (1) defaulted copy-ctor (defective)
  /*f6*/ Vector(Vector &&vec) = /*b*/ default /*x*/;      // (2) defaulted move-ctor (defective)
  ~/*f6*/ Vector() = /*b*/ default /*x*/;                 // (3) defaulted dtor (defective)
  value_type *data() { return dataPointer; }
  size_type size() { return dataSize; }
  value_type &operator[](size_type pos) { return dataPointer[pos]; }
};
```
Prepare yourself for a discussion of your implementation. 

## Benchmarking
For testing your implementation and its performance, a benchmark is provided in `benchmarking_constructors.cpp`. It measures the run time of
```pmans
    Widget var = Widget(size, std::rand());
    Widget w = /*f*/ std::move /*x*/(var) ; // invoking move construction
```
and
```pmans
    Widget var = Widget(size, std::rand());
    Widget w = var; // invoking copy construction
```    
for the copy/move constructors of `Widget` (and its member `Vector`).

## Sanitizing
The provided `CMakeLists.txt` builds the benchmark and turns on the AddressSanitizer (gcc/clang) to detect memory related problems (e.g., leaks and out-of-bound accesses, see [Item 001](https://cppitems.github.io/#/item/001)). 
We will use this to detect problems in your submitted implementation and it is recommended that you also use the AddressSanitizer (or a equivalent tool on you platform) for tests during development.

## Submission
Finally, you should submit EX1 (all three parts) using a single repository `ex1`.
Therefore you repo should look like this after EX1.2:
```
benchmark_initializers.cpp
benchmark_constructors.cpp
CMakeLists.txt
vector.hpp
```
Further you should add all relevant targets in the `CMakeLists.txt` which should look similar to this after EX1.2:

```
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
project(ex11ex12 LANGUAGES CXX)
add_executable(benchmark_initializers benchmark_initializers.cpp)
add_executable(benchmark_constructors benchmark_constructors.cpp) # this line is new
```
