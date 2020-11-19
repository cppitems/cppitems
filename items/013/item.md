1 // item status
# Exercise EX2.3

This exercise is part **three** of three parts of EX2. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX2 (all three parts) is **Mo 30.11.2020, 4pm**. The sources related to EX2.3 are available at [Item 013](https://github.com/cppitems/cppitems/tree/master/items/013).

## Task description

In this exercise, it is your task to implement a wrapper-functionality to measure the run time of "callables".
The syntax of how to measure the run time is already fixed and provided in `test_timer.cpp` ([here](https://raw.githubusercontent.com/cppitems/cppitems/master/items/013/test_timer.cpp)).
More specifically, you have to implement the wrapping-functionality solely in `timer.h` and are not allowed to change the interface to the wrapper used in `test_timer.cpp` .
 Further, your implementation has to support all use cases present in `test_timer.cpp` . 
This means: After you completed your implementation in `timer.h` the project should compile without any adoptions in `test_timer.cpp`.

For example, to track the run time of the function `func4` , it looks like this in `test_timer.cpp`:
```pmans
/* file: test_timer.cpp */
#include "timer.h" // put your implementation of `track_time` in this header
template <typename T> void /*f*/ func4 /*x*/(const std::vector<T> &a, const std::vector<T> &b,
           std::vector<T> &res) {...}
...
int main(){
  ...
  /*f*/ func4<T> /*x*/(/*b*/ vec1 /*x*/, /*b*/ vec2 /*x*/, /*b*/ vec3 /*x*/); // regular call
  track_time(/*f*/ func4<T> /*x*/, /*b*/ vec1 /*x*/, /*b*/ vec2 /*x*/, /*b*/ vec3 /*x*/); // call using wrapper which tracks run time
}
```
Similarly, measuring the run time of a lambda (function object) looks like this:
```pmans
/* file: test_timer.cpp */
#include "timer.h"
...
int main(){
  ...
  auto /*f*/ lambda_fill /*x*/ = [](auto &ctnr) {
    std::fill(ctnr.begin(), ctnr.end(), double{1});
  };
  /*f*/ lambda_fill /*x*/(/*b*/ vec /*x*/); // regular call
  track_time(/*f*/ lambda_fill /*x*/, /*b*/ vec /*x*/); // call using wrapper which tracks time
}
```
**Hint**: Implementing the wrapper-functionality using 
- **perfect forwarding** (to support any combination of "value-ness", "const-ness" and "reference-ness" for the list of arguments) in combination with a
- **variadic template** (to be able to forward any number of arguments)

is the method of choice in this scenario (to avoid code duplication and support a wide range of function calls).

**Note**: For this exercise, you can assume that the `track_time` wrapper-functionality only needs to support functions with return type `void`.

Prepare yourself for a discussion on your submission.

### Output of run times
Your implementation should output the run time of each "wrapped call" to the console.
Additionally, the individual run times of the calls are accumulated; this accumulated run time should also be reported to the console after each "wrapped call". 
So the output should look similar to this:
```
using T = double;
call took: 1.610445e-01s, accumulated: 1.610445e-01
call took: 3.155593e-01s, accumulated: 4.766038e-01
call took: 3.270131e-01s, accumulated: 8.036169e-01
call took: 3.232206e-01s, accumulated: 1.126837e+00
using T = float;
call took: 7.906291e-02s, accumulated: 1.205900e+00
call took: 1.168161e-01s, accumulated: 1.322717e+00
call took: 3.634743e-01s, accumulated: 1.686191e+00
call took: 3.374491e-01s, accumulated: 2.023640e+00
lambda function object
call took: 1.004630e-02s, accumulated: 2.033686e+00
```

## Testing
As already noted above, `test_timer.cpp` contains several use cases of the wrapping-functionality you implement in `timer.h`. We will use `test_timer.cpp` to test your submission.

Note: You should not change any code in `test_timer.cpp`; we will use an unmodified version to test your submission.

**Hint**: When you start working on this exercise, the project will no compile without error. You can use the compiler argument "-fmax-errors=1" to stop compilation after the first error to prevent "meter-long" error messages w.r.t. failed template instatiations:
```pmans
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall /*f*/ -fmax-errors=1 /*x*/" CACHE STRING "")
```

## Submission
Finally, you should submit EX2 (all three parts) using a single repository `ex2`.
Therefore you repo should look like this after EX2.3:
```
test_algorithms.cpp
algorithms.h
test_timer.cpp
timer.h
test_distribution.cpp
Distribution.hpp
SpaceVector.hpp
CMakeLists.txt
```
Further you should add all relevant targets in the `CMakeLists.txt` which should look similar to this after EX2.3:
```
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "-std=c++17 -Werror -Wall" CACHE STRING "")
project(ex2 LANGUAGES CXX)
add_executable(test_algorithms test_algorithms.cpp) # this line is for EX2.1
add_executable(test_distribution test_distribution.cpp) # this line is for EX2.2
add_executable(test_timer test_timer.cpp) # this line is for EX2.3
```

