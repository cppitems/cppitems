1 // item status
# Exceptions
Let's first consider C (which does not have exceptions). But of course the desire to capture and handle unforeseen events is present. A pattern to achieve this is to always use the return value of a function as error indication. Let's introduce this pattern for a `grid_init` function to capture problems during allocation which we adopt from
```pmans
/*b*/ void /*x*/ grid_init(struct Grid *grid, size_t nx, size_t ny) {
  grid->data = (double *)calloc(nx * ny, sizeof(double));
  ...  
}        
// usage example
grid_init(&grid, 3, 4);    
```
to a version returning an error code instead of void:
```pmans
/*b*/ int /*x*/ grid_init(struct Grid *grid, size_t nx, size_t ny) {
  void * ptr = (double *)calloc(nx * ny, sizeof(double));
  if(ptr == NULL)
     /*b*/ return 101; /*x*/       // error code for bad alloc
  grid->data = ptr;
  ...  
  return 0;            // success
}
```
Now also the call site has to be adopted to capture (and propagate) the error code:
```pmans
int main() {
  ...
  int error = grid_init(&grid, 3, 4);
  if (!error) { 
    ...
    grid_free(&grid);
    /*b*/ return 0; /*x*/    // success
  }
  /*b*/ return error; /*x*/   // forward error code
}
```
Essentially, this pattern of error code handling forces many functions to return an error code: obviously those function which do things that are error-prone (like allocation, or user input) but also all other functions which have to forward error codes of other functions. This is necessary to pass the error information up the call stack. This approach is not ideal, as the source code in many functions is concerned about error code logic, even if the error is just forwarded:
```pmans
int func() {
  int error1 = func1();     // might error (or just forwarding)
  int error2 = func2();     // might error (or just forwarding)
  if (error1 | errro2)
    return error1 & error2; // forward both errors
  return 0;                 // no errors
} 
```
and also the return value is always utilized to forward the error codes, and cannot be used for other purposes.

> Are there alternative approaches in C beside using the return value to propagate errors?
<!--
- yes: using a global function which allows the caller to check for the most recently reported error of an API.
-->

C++ provides an extra mechanism to handle such scenarios: **exceptions**. To see how they work, let's adopt the above grid example to an exception:
```pmans
/*b*/ void /*x*/ grid_init(Grid *grid, size_t nx, size_t ny) {
  void *ptr = /*b*/ std:: /*x*/calloc(nx * ny, sizeof(double));
  if (ptr == NULL)
     /*b*/ throw "bad alloc"; /*x*/
  grid->data = (double *)ptr;
  ...
}
```

> Which objects can be "thrown" and where is this allowed? 
<!--
> - any object (copyable), anywhere in the code
-->


> Where is the exception reported/catched?

The normal execution is interrupted at the point of `throw ...` and the exception object is created/constructed. 
In our case the exception object is a string literal "bad alloc" which provides minimal information about what happened. Of course also more complex exceptions objects can be created, e.g., the standard library provides `std::bad_alloc` which inherits from `std::exception`. 

The execution of the program is then continued at the containing (and matching) catch block of a `try{ } catch(...){ }` statement that is found.
In our example we adopt the main function body to catch exceptions that match our exception object:
```pmans
int main() /*b*/ try { /*x*/
  grid_init(&grid, 3, 4);            // can throw
  throw "error";    // throw string literal
  grid_free(&grid);  
  return 0;
/*b*/ } catch (char const *e) { /*x*/           // matching catch block
  ...                                // output debug information 
  return 0;                          // terminate without error
/*b*/ } /*x*/ 
```

If no matching catch block is found in the call stack, the program is terminated (`std::terminate`).

> Are exceptions "lightweight"? 
<!--
> - yes and no
> - no: undwinding is heavy-lifting 
> - no: binary size for exception handling tables)
> - yes: runtime overhead low if no exceptions are triggered
-->
Exceptions provide a powerful mechanism to unwind the call stack until a matching catch block is found. During unwinding a lot of local variables/objects go out of scope and are automatically destructed (the destructor is called) according to the rules of the C++ standard.

> Can more than one exception be active at the same time? If no, what are immediate consequences
<!--
> - no
> - destructors should not throw (in general: dellocation routines should never be interrupted by an exception)
-->

> Do you find an example for a "dellocation routine" in EX3 which is not a destructor?
<!--
> - unique_ptr::reset(...)
> - shared_ptr assignment operators
> - 
-->

The intended use of exceptions is to react on (rare) cases where there is an issue with an external resource (e.g., access to a file or memory allocation). 
Valuable debug information can be collected before stack unwinding (part) of an application.

> How do asserts compare to exceptions?
<!--
> - asserts: check for situations which are not expected to be able to happen (during debug/tests)
> - exceptions: use to enable separation between code that throws and the code that handles the problem
> - exceptions: should capture rare events -> unwind overhead is not critical
-->

## Exception Safety
Common exception safety levels (not part of the C++ standard) can be used to characterize functions by additional information about the side-effects of a function if an exceptions occurs:
- **no guarantee**: an exception within the function might lead to an invalid state.
```pmans
void no_guarantee(){
  int *m = new int();
  ++(*m);
  /*b*/ throw "leak"; /*x*/
  delete m;
}
```
- **basic guarantee**: an exception will not lead to an invalid state, but the state might have changed. (this is what you always want at least, e.g. proper deconstruction)
```pmans
void basic_guarantee(){
  /*f*/ ++global; /*x*/
  std::unique_ptr<int> m = std::make_unique<int>();
  ++(*m);
  /*b*/ throw "noleak"; /*x*/
}
```
- **strong guarantee**: the state of the program is valid and identical to the state before the call.
```pmans
void strong_guarantee(){
  try {
    std::unique_ptr<int> m = std::make_unique<int>();
    ++(*m);
    /*b*/ throw "strong"; /*x*/
  } catch (...) {
    ... // rollback
    /*b*/ throw; /*x*/ //rethrow
  }
  /*f*/ ++global; /*x*/
}
```
- **no fail**: no exception will propagate to the outside. 
(or: if an exception is propagates it is ok to terminate)
```pmans
void no_fail() /*b*/ noexcept /*x*/ {
  try {
    throw "error";
  } catch (...) {
    // handle error
  }    
}
```
> What happens if an exception is thrown in a `noexcept` function?
<!--
> - termination
-->

> Why is `noexcept` useful?
<!--
> - optimization: moving elements into stdlib container (if move ctor of `T` is not tagged `noexcept`, copies are made)
>    - why so: a potentially throwing-moving would not allow a rollback (as this could throw again and therefore terminate)
> - compiler 
-->

> Are any functions implicitly defined `noexcept`?
<!--
> - yes, default SMFs for construction and assignment are `noexcept` if the involved (base)class members' SMFs are `noexcept`
-->

> Can a constructor of a type which requires dynamic allocation of members be `noexcept`?
<!--
> - not really, but there is a trick, let's look at `std::vector`
-->

### Example

Let's have a look at a copy assignment operator implementation for a `shared_ptr`:
```pmans
struct Widget{
    int *m;
    Widget(int init) : m(new int) {
        *m = init;
    };
    ~Widget(){delete m;}
    ...
    // (1)
    Widget& operator=(const Widget& other){
        delete this->m;
        this->m = new int;
        *(this->m) = other->m;
        return *this;
    }
    // (2)
    Widget& operator=(const Widget& other){
        Widget(other).swap(*this); 
        return *this;
    }    
}
```
> What about exception safety of the involved functions in the above snippet?

# CMake revisited
CMake is a configuration generator.
It is not a build-system/toolchain -- but it **knows** everything about some build-systems/toolchains.

> Examples for build-systems/toolchains?
<!--
> - make, Xcode, VS
-->

CMake generates configurations for the supported build-systems.
It provides ways to formulate/encode a high-level logical project configuration in a generic way using its own language (as far as possible, e.g., some things map directly to options/settings in a specific toolchain). 
This logical project configuration is then used to generate configurations for different build-systems. 

> Does CMake support other languages than C/C++?
<!--
> - focus C/C++ Fortan/ObjC(Apple)/swift
-->

> How should much should the release date of your compiler and CMake you use differ?
<!--
> - cmake should be a bit more recent than compiler
-->

Currently, for C++, there is no practical alternative to CMake.

You have seen many CMakeLists.txt files in the exercises: It is not required to understand all details in a CMake configuration in order to use it.

> When do you have to understand details of the CMake config of a dependency?
<!--
> - if the CMake config does not allow you to build/use the project suitable for your use case
-->

In general, CMake files can be considered as source code and all general best practices apply, too:
- consistent style and conventions
- redundancy is error-prone
- file organization 

### Modern CMake 
"Modern CMake" (~2018/3.12) characterizes a *target-centric* approach to describe configurations: 
- everything is a target (think of a C++ class)
- properties of targets can be configured (think of C++ members/setters)
- one group of properties of a target is concerned with how and from what it should be build: **build-requirements**
- another group of properties is concerned with how a target can be used (after it is build/installed): **usage-requirements**

> Examples for different categories of **build-requirements**?
<!--
> - source files, link/inlcude dirs, compiler options, preprocessor defines
-->

> Target properties can be *transitive*. What does that mean?
<!--
> - if targets depend on each other this propagates the requirements
-->

Alternative to the *target-centric* the "classic" CMake way is to use (many)variables which are not immediately coupled to a specific target.

### Use cases for CMake
If a project uses/supports CMake this can mean different things:
- using CMake "only" to build binaries
- using CMake to install libraries
- providing CMake support for clients/users of the project
- using CPack 
- using CTest

> What can be done if a project does not support CMake but needs to be integrated in a larger CMake based project?
<!--
> - write a custom FindXXX.cmake  `find_package(XXX)`
-->

### Project boundaries/modularity
It might happen that a project provides a "monolithic CMake support": many things (e.g., sub-projects) are configured in a non-modular way. 
The "Modern" CMake approach can help to avoid/resolve such situations:
Each target can exits on its own (being build and consumed), declaring dependencies between targets allows to configure more complex projects.

> If you have to patch a CMakeLists.txt of a project you depend on, is this a good sign? If not, what is the "right" way to customize a CMake based project you consume?
<!--
> - passing commandline arguments/setting cmake variables 
-->

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
- C++ exceptions: https://en.cppreference.com/w/cpp/language/exceptions
- Exceptions Best Practices: https://docs.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp
- CMake Wiki: https://gitlab.kitware.com/cmake/community/-/wikis/home
- CMake tutorial: https://cmake.org/cmake/help/v3.18/guide/tutorial/index.html
- CMake for library authors (2019, Craig Scott): https://www.youtube.com/watch?v=m0DwB4OvDXk
- "Modern CMake" talks (2018/19, Deniz Bahadir): https://www.youtube.com/watch?v=y7ndUhdQuU8 and https://www.youtube.com/watch?v=y9kSr5enrSk
- Effective CMake (2017, Daniel Pfeifer): https://www.youtube.com/watch?v=bsXLMQ6WgIk
- CMake config example: https://dominikberner.ch/cmake-interface-lib/