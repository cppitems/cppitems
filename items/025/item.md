1 // item status
# C++ Standards
Brief excerpt of recent history of language and stdlib features
> Let's go through the selection of features below and try to give them a name!
## C++11
### Language
```pmans
/*b9*/ constexpr int factorial(int n) { ... } 
const int tmp = factorial(3);
```
```pmans
struct Widget {
   int a,b,c;
};
...
/*b4*/ auto w = Widget/*b*/ {1,2,3} /*x*/; 
```
```pmans
template<typename/*b*/ ... ARGS /*x*/> void f(ARGS... args) {} 
```
```pmans
auto lambda = /*b*/ [](int a){ return a+1; } /*x*/;
```
```pmans
for(const auto& item /*b1*/ : items){ ... }
```
```pmans
void f() /*b8*/ noexcept; 
```
```pmans
/*b8*/ decltype(Widget) w;
```
```pmans
/*b8*/ decltype(Widget) w;
```
```pmans
void func(Widget /*b2*/ &&rval);
```
```pmans
struct Widget{
    Widget(Widget&& other) = /*b7*/ default;
    Widget& operator=(Widget &&other) = /*b7*/ default;
}
```

### Library
```pmans
#include <type_traits>
std::cout << std::/*b*/ is_floating_point /*x*/<double>::value << std::endl;
```
```pmans
#include <memory>
std::/*b*/ shared_ptr /*x*/
std::/*b*/ unique_ptr /*x*/
#include <thread>
std::/*b*/ thread  /*x*/
std::/*b*/ future  /*x*/
std::/*b*/ promise  /*x*/
std::/*b*/ async  /*x*/
```
## C++14
### Language
```pmans
auto lambda = [](/*b4*/ auto a, /*b4*/ auto&& b) { return a < b; };
```
... and fixes for C++11
### Library
```pmans
std::/*b*/ make_unique /*x*/
```
... and fixes for C++11

## C++17
### Language
```pmans
if /*b*/ constexpr /*x*/ (std::is_integral<T>::value) {
  return true;
} else {
  return false;
}
```
```pmans
template <typename T>
struct Widget {
  T m;
}
auto w = /*b*/ Widget{1} /*x*/;
```
### Library
```pmans
// std::any
// std::optional
// std::variant
// std::invoke
// std::apply
auto result2 = std::sort(/*b*/ std::execution::seq /*x*/, being, end);
```
## C++20
### Language
```pmans
auto wrapper = [] /*b*/ <typename... ARGS> /*x*/ (ARGS && ...args) {
  return foo(std::forward<ARGS>(args)...);
};
```
```pmans
struct Widget {
  int m;
  auto /*b*/ operator<=> /*x*/(const Widget &) const = default;
};
```
```pmans
struct Widget {
  int m1,m2;
};
Widget w {/*b3*/ .m1 = 1, /*b3*/ .m2 = 2};
```
```pmans
template <class T>
/*b*/ concept /*x*/ Integral = std::is_integral_v<T>;
```
- coroutines
- ranges
- ... improvements
### Library
- concepts library
- ranges library

### Where to look at?
- https://en.cppreference.com/w/cpp/links
- https://en.cppreference.com/w/cpp/compiler_support

# CMake (revisited)
CMake is a configuration generator.
It is not a build-system/toolchain -- but it **knows** everything about some build-systems/toolchains.

> Examples for build-systems/toolchains?

CMake generates configurations for the supported build-systems.
It provides ways to formulate/encode a high-level logical project configuration in a generic way using its own language (as far as possible, e.g., some things map directly to options/settings in a specific toolchain). 
This logical project configuration is then used to generate configurations for different build-systems. 

> Does CMake support other languages than C/C++?

> How should much should the release date of your compiler and CMake you use differ?


Currently, for C++, there is no practical alternative to CMake.

You have seen many CMakeLists.txt files in the exercises: It is not required to understand all details in a CMake configuration in order to use it.

> When do you have to understand details of the CMake config of a dependency?


In general, CMake files can be considered as source code and all general best practices apply, too:
- consistent style and conventions
- redundancy is error-prone
- file organization 

## Modern CMake 
"Modern CMake" (~2018/3.12) characterizes a *target-centric* approach to describe configurations: 
- everything is a target (think of a C++ class)
- properties of targets can be configured (think of C++ members/setters)
- one group of properties of a target is concerned with how and from what it should be build: **build-requirements**
- another group of properties is concerned with how a target can be used (after it is build/installed): **usage-requirements**

> Examples for different categories of **build-requirements**?


> Target properties can be *transitive*. What does that mean?


Alternative to the *target-centric* the "classic" CMake way is to use (many)variables which are not immediately coupled to a specific target.

## Use cases for CMake
If a project uses/supports CMake this can mean different things:
- using CMake "only" to build binaries
- using CMake to install libraries
- providing CMake support for clients/users of the project
- using CPack 
- using CTest

> What can be done if a project does not support CMake but needs to be integrated in a larger CMake based project?

## Project boundaries/modularity
It might happen that a project provides a "monolithic CMake support": many things (e.g., sub-projects) are configured in a non-modular way. 
The "Modern" CMake approach can help to avoid/resolve such situations:
Each target can exits on its own (being build and consumed), declaring dependencies between targets allows to configure more complex projects.

> If you have to patch a CMakeLists.txt of a project you depend on, is this a good sign? If not, what is the "right" way to customize a CMake based project you consume?


## Example
Let's look at an example to illustrate the *target-centric* approach:
- The `Widget` Project contains a library and an associated test
- The `UseWidget` Project depends on the library provided by `Widget`

> Hands-On
```
widget > cmake -S . -B build -L -D CMAKE_INSTALL_PREFIX=`pwd`/../install -D CMAKE_BUILD_TYPE=Debug
widget > cmake --build build
widget > cmake --install build
usingwidget > cmake -S . -B build 
usingwidget > cmake --build build
```

## Links
- CMake Wiki: https://gitlab.kitware.com/cmake/community/-/wikis/home
- CMake tutorial: https://cmake.org/cmake/help/v3.18/guide/tutorial/index.html
- CMake for library authors (2019, Craig Scott): https://www.youtube.com/watch?v=m0DwB4OvDXk
- "Modern CMake" talks (2018/19, Deniz Bahadir): https://www.youtube.com/watch?v=y7ndUhdQuU8 and https://www.youtube.com/watch?v=y9kSr5enrSk
- Effective CMake (2017, Daniel Pfeifer): https://www.youtube.com/watch?v=bsXLMQ6WgIk
- CMake config example: https://dominikberner.ch/cmake-interface-lib/