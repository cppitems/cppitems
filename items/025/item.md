1 // item status
# C++ Standards

> What is actually a "C++ Standard"?
> - a specification of language features **and** w.r.t. to the standard libary interfaces
> - https://en.cppreference.com/w/cpp/links -> useful links
> - https://en.cppreference.com/w/cpp/compiler_support -> current status w.r.t. compiler support


> How does a "C++ Standard" evolve?
> - The committee decides: http://open-std.org/jtc1/sc22/wg21/ , you can
> - 1. have idea for improvement
> - 2. write a proposal and send to committee
> - 3. iterate your proposal due to feedback
> - 4. idea can become/influence a future feature of C++
> - recent example: C++ format strings: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0645r10.html

> Is a compiler allowed to support non-standard extensions/behaivour?
> - yes (gcc)
> - why? (convenience)

> Is there a disadvantage if your rely on non-standard extensions/behaivour? 
> - not portable to other toolchains compilers

> Do all compilers (plan to) implement all language/library features?
> - core language features: probably yes; with different priorities though
> - library features: probably not all compiler will can the full set (e.g., it might not be possible/useful to implement certain functionality for certain compile targets)

> If a compiler developer discovers a "bug", what to do? 
> - write a defect report and submit to the committee

> Which standard should I use?
> - if no other constraints: the newest one fitting for "all involved parties"

Below list is a brief excerpt of recent history of language and stdlib features.
Let"s try to tag all the snippets below by providing a name for each feature! 

> Insert: What was there before C++11?
> - C++03 (super old!)
> - the Boost-libraries: a collection of upcomping C++ stdlib feature -> an "inofficial extension of the stdlib"

## C++11
### Language
```pmans
/* file: (1) constexpr */
/*b9*/ constexpr int factorial(int n) { ... } 
const int tmp = factorial(3);
```
```pmans
struct Widget {
   int a,b,c;
};
...
/* file: (2) auto type deduction and list init */
/*b4*/ auto w = Widget/*b*/ {1,2,3} /*x*/; 
```
```pmans
/* file: (3) template parameter pack */
template<typename/*b*/ ... ARGS /*x*/> void f(ARGS... args) {} 
```
```pmans
/* file: (4) lambda expressions */
auto lambda = /*b*/ [](int a){ return a+1; } /*x*/;
```
```pmans
/* file: (5) range-based for loop */
for(const auto& item /*b1*/ : items){ ... }
```
```pmans
/* file: (6) noexcept */
void f() /*b8*/ noexcept; 
```
```pmans
/* file: (7) decltype */
/*b8*/ decltype(other) w;
```
```pmans
/* file: (8) rvalue references and move semantics */
void func(Widget /*b2*/ &&rval);
```
```pmans
/* file: (9) default versions for move semantics */
struct Widget{
    Widget(Widget&& other) = /*b7*/ default;
    Widget& operator=(Widget &&other) = /*b7*/ default;
}
```

### Library
```pmans
/* file: (1) type traits library */
std::cout << std::/*b*/ is_floating_point /*x*/<double>::value << std::endl;
```
```pmans
/* file: (2) smart pointers */
std::/*b*/ shared_ptr /*x*/
std::/*b*/ unique_ptr /*x*/
```
```pmans
/* file: (3) thread support  */
std::/*b*/ thread  /*x*/
std::/*b*/ future  /*x*/
std::/*b*/ promise  /*x*/
std::/*b*/ async  /*x*/
```
## C++14
### Language
```pmans
/* file: (1) generic lambda #1 */
auto lambda1 = [](/*b4*/ auto a, /*b4*/ auto&& b) { return a < b; }; // lambda_from_line_X_in_main.cpp
auto lambda2 = [](/*b4*/ auto a, /*b4*/ auto&& b) { return a < b; }; // lambda_from_line_X+1_in_main.cpp 
```
... and fixes for C++11
### Library
```pmans
/* file: (2) convenience and performance */
std::/*b*/ make_unique /*x*/
```
... and fixes for C++11

## C++17
### Language
```pmans
/* file: (1) if constexpr */
/*b*/ if constexpr /*x*/ (std::is_integral<T>::value) {
  return (T{} << 1);
} else {
  return T{};
}
```
```pmans
/* file: (2) auto class template type deduction and deduction guide */
template <typename T>
struct Widget {
  T m;
}
auto w = /*b*/ Widget{1} /*x*/;
// auto w = /*b*/ Widget<int>{1} /*x*/;
```
### Library
```pmans
// we did no cover any of those:
// std::any
// std::optional
// std::variant
// std::invoke
// std::apply

/* file: (1) parallel algorithms library via tag */
auto result2 = std::sort(/*b*/ std::execution::par /*x*/, being, end);
```
## C++20
### Language
```pmans
/* file: (1) generic lambdas #2 */
auto wrapper = [] /*b*/ <typename... ARGS> /*x*/ (ARGS && ...args) {
  return foo(std::forward<ARGS>(args)...);
};
```
```pmans
/* file: (2) three-way comparison aka space-ship operator and defaults */
struct Widget {
  int m;
  auto /*b*/ operator<=> /*x*/(const Widget &) const = default;
};
```
```pmans
/* file: (3) named init */
struct Widget {
  int m1,m2;
};
Widget w {/*b3*/ .m1 = 1, /*b3*/ .m2 = 2};
```
```pmans
/* file: (3) concepts to streamline SFINAE constructs and constraints */
template <class T>
/*b*/ concept /*x*/ Integral = std::is_integral_v<T>;

template <class T>
/*b*/ requires /*x*/ Integral<T>
void function(T input){

}

```
Further C++20 language features
- coroutines
- ranges
- ...and many improvements

### Library
- concepts library
- ranges library


# CMake (revisited)
CMake is a configuration generator.
It is not a build-system/toolchain -- but it **knows** everything about some build-systems/toolchains.

> Examples for build-systems/toolchains?
> - make, Xcode, VS, ninja, boost.build (b2), Scons


CMake generates configurations for the supported build-systems.
It provides ways to formulate/encode a high-level logical project configuration in a generic way using its own language (as far as possible, e.g., some things map directly to options/settings in a specific toolchain). 
This logical project configuration is then used to generate configurations for different build-systems. 

> Does CMake support other languages than C/C++?
> - focus assembly/C/C++ Fortan/ObjC(Apple)/swift


> How should much should the release date of your compiler and CMake you use differ?
> - cmake should be a bit more recent than compiler

Currently, for C++, there is no practical alternative to CMake.

You have seen many CMakeLists.txt files in the exercises: It is not required to understand all details in a CMake configuration in order to use it.

> When do you have to understand details of the CMake config of a dependency?
> - if the CMake config does not allow you to build/use the project suitable for your use case


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
> - source files, link/inlcude dirs, compiler options, preprocessor defines


> Target properties can be *transitive*. What does that mean?
> - if targets depend on each other this propagates the requirements


Alternative to the *target-centric* the "classic" CMake way is to use (many)variables which are not immediately coupled to a specific target.

## Use cases for CMake
If a project uses/supports CMake this can mean different things:
- using CMake "only" to build binaries
- using CMake to install libraries
- providing CMake support for clients/users of the project
- using CPack 
- using CTest

> What can be done if a project does not support CMake but needs to be integrated in a larger CMake based project?
> - write a custom FindXXX.cmake  `find_package(XXX)`
> - use `external_project` 


## Project boundaries/modularity
It might happen that a project provides a "monolithic CMake support": many things (e.g., sub-projects) are configured in a non-modular way. 
The "Modern" CMake approach can help to avoid/resolve such situations:
Each target can exits on its own (being build and consumed), declaring dependencies between targets allows to configure more complex projects.

> If you have to patch a CMakeLists.txt of a project you depend on, is this a good sign? If not, what is the "right" way to customize a CMake based project you consume?
> - passing commandline arguments/setting cmake variables 

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

## links
- CMake Wiki: https://gitlab.kitware.com/cmake/community/-/wikis/home
- CMake tutorial: https://cmake.org/cmake/help/v3.18/guide/tutorial/index.html
- CMake for library authors (2019, Craig Scott): https://www.youtube.com/watch?v=m0DwB4OvDXk
- "Modern CMake" talks (2018/19, Deniz Bahadir): https://www.youtube.com/watch?v=y7ndUhdQuU8 and https://www.youtube.com/watch?v=y9kSr5enrSk
- Effective CMake (2017, Daniel Pfeifer): https://www.youtube.com/watch?v=bsXLMQ6WgIk
- CMake config example: https://dominikberner.ch/cmake-interface-lib/