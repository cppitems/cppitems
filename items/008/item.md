1 // item status
# Exercise EX1.3
This exercise is part **three** of three parts of EX1. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX1 (all three parts) is **Mo 09.11.2020, 4pm**. The sources related to EX1.3 are available at [Item 008](https://github.com/cppitems/cppitems/tree/master/items/008).

## Task description

In this exercise, it is your task to extended a (very basic) implementation `List` of a single-linked-list (provided in `list.hpp`).
More specifically, the provided implementation in `list.hpp` comprises a user-defined constructor and correct user-defined destructor.

It is your task to implement the remaining special member functions for this resource owning class `List`, which are *deleted* in the provided implementation:
- (1) the copy constructor,
- (2) the move constructor,
- (3) the copy assignment operator, and
- (4) the move assignment operator.

You should only change code of these four special member functions.
The declaration of `List` you start with looks like this:
```pmans
template <class /*b1*/ T> class /*f4*/ List {
public:
  using /*f*/ value_type /*x*/ = /*b1*/ T;
  using size_type = std::size_t;

  // nested class
  struct /*f4*/ Node {
    /*f*/ value_type /*x*/ value;
    /*f4*/ Node *next;
    /*f4*/ Node(const /*f*/ value_type & /*x*/val);
  };

private:
  /*f4*/ Node *dataHead; // root element
  size_type dataSize;

public:
  // default ctor
  /*f4*/ List();
  // custom ctor
  /*f4*/ List(size_type count, const /*f*/ value_type & /*x*/value);

  // you task it to implement the four currently deleted SMFs (1,2,3,4) below
  // according to the needs of this resource owning class

  /*f4*/ List(const /*f6*/ List &other) = /*b6*/ delete;                  // (1) copy-ctor
  /*f4*/ List(/*f7*/ List &&other) = /*b6*/ delete;                       // (2) move-ctor
  /*f4*/ List &operator=(const /*f6*/ List &other) = /*b6*/ delete;       // (3) copy-assignment
  /*f4*/ List &operator=(/*f7*/ List &&other) = /*b6*/ delete;            // (4) move-assignment

  // dtor
  ~List();

  size_type size();
  /*f4*/ Node *data();
  /*f*/ value_type & /*x*/front();
  void push_front(const /*f*/ value_type & /*x*/value);
  void pop_front();
};
```
Prepare yourself for a discussion of your implementation. 

## Benchmarking
For testing your implementation and its performance, a benchmark is provided in `benchmarking_list.cpp`. 
It measures and compares the run time for
- copy construction
```pmans
    using ListD = List<double>;
    ListD var = ListD(size, std::rand());
    ListD w /*b1*/ = var; // invoking copy-ctor
```   
- move construction:
```pmans
    
    ListD var = ListD(size, std::rand());
    ListD w /*b1*/ = /*f9*/ std::move(var); // invoking move-ctor
```
- copy assignment:
```pmans
    ListD var = ListD(size, std::rand());
    ListD w;
    w /*b1*/ = var; // invoking copy-assign
```
- move assignment:
```pmans
    ListD var = ListD(size, std::rand());
    ListD w;
    w /*b1*/ = /*f9*/ std::move(var); // invoking move-assign
```

## Sanitizing
The provided `CMakeLists.txt` builds the benchmark and turns on the AddressSanitizer (gcc/clang) to detect memory related problems (e.g., leaks and out-of-bound accesses, see [Item 001](https://cppitems.github.io/#/item/001)). 
We will use this to detect problems in your submitted implementation and it is recommended that you also use the AddressSanitizer (or a equivalent tool on you platform) for tests during development.

## Submission
Finally, you should submit EX1 (all three parts) using a single repository `ex1`.
Therefore you repo should look like this after EX1.3:
```
benchmark_initializers.cpp
benchmark_constructors.cpp
benchmark_list.cpp
CMakeLists.txt
vector.hpp
list.hpp
```
Further you should add all relevant targets in the `CMakeLists.txt` which should look similar to this after EX1.3:

```
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
project(ex1 LANGUAGES CXX)
add_executable(benchmark_initializers benchmark_initializers.cpp)
add_executable(benchmark_constructors benchmark_constructors.cpp) 
add_executable(benchmark_list benchmark_list.cpp) 
```
