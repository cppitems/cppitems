1 // item status
# Exercise EX3.2

This exercise is part **two** of three parts of EX3. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX3 (all three parts) is **Mo	11.01.2021, 4pm**. The sources related to EX3.2 are available at [Item 018](https://github.com/cppitems/cppitems/tree/master/items/018).

## Task description
In this exercise it is your task to complete the implementation of a *smart pointer* class according to requirements specified using provided test cases. 
The class you should implement needs to provide functionality similar to a `std::unique_ptr`.
An minimal starting point is provided in `unique_ptr.hpp`:
```pmans
template <typename T> class /*f*/ unique_ptr /*x*/ {
  T *ptr;

public:
  // construction
  unique_ptr(T *p) : ptr(p) {}

  // destruction
  ~unique_ptr() { delete ptr; }

  // access
  T *get() { return ptr; }
  T &operator*() { return *get(); }
  T *operator->() { return get(); }
};
```
As you can see, this `unique_ptr` is a simplified version of `std::unique_ptr` as it always uses `delete` to dealloacte the managed resource. 
It does not support arrays (i.e., `delete[]`). This also means implicit type deduction is possible (which is not available for `std::unique_ptr`).

It is your task to implement the missing functionality to pass all provided tests:
- move construction (TestA) and move assignment (TestB) 
- disable copy construction and disable copy assignment (TestB) 
- assignment from `nullptr` (TestB)
- member `release()` to stop management of the currently wrapped resource and return a pointer to the resource (TestC)
- member `reset(...)` to deallocate the currently managed resource and start managing the resource passed as argument via a pointer (TestC)
- comparison operators `==` and `!=` between two `unique_ptr`s of the same type (TestD)
- comparison operators `==` and `!=` between a `unique_ptr` and a `nullptr` (TestD)

### Custom deleter
Finally, you are required to add functionality to support a *custom deleter* (TestE and TestF) which takes a pointer to the resource as argument.
One (recommended) way to achieved this is using a `std::function` with a fixed signature `void(T *)` to capture a custom deleter as a member:
```pmans
template <typename T> class /*f*/ unique_ptr /*x*/ {
  T *ptr;
  /*b*/ std::function<void(T *)> /*x*/ /*f*/ deleter /*x*/;  
  ...
}
```
The deleter can then be used with `del(ptr);` instead of `delete ptr;` . 

**Hint**: It is a valid approach to first neglect the requirements for a custom deleter (TestE and TestF) and (at first) only implementing the requirements for TestA-TestD; once these tests work you can extend your `unqiue_ptr` to also support a custom deleter.

**Note**: If you desire more granular tests you can also comment parts of a test case during development.

## Requirements and testing
As for EX3.1, we provide tests to specify the desired functionality and to guide the implementation.
The tests are provided in [SmartPtrTests](https://github.com/cppitems/cppitems/tree/master/items/018/SmartPtrTests).
After running CMake in your build folder, you can build all tests using:
```bash
make
```
or just one test using the name of the test:
```bash
make TestA_construction
```
Once all tests compile (and complete successfully), you can run them all after each other with the provided shell script `runSmartPtrTests.sh`. 
Your `unique_ptr` implementation should pass all tests successfully.

**Note**: We will test your implementation with the AddressSanitizer turned on. You can do so by configuring you project using `cmake -D CMAKE_BUILD_TYPE=Debug ..` which adds the sanitizer flags in the supplied CMake configuration.

## References
- `std::unique_ptr`: https://en.cppreference.com/w/cpp/memory/unique_ptr
- `std::function`: https://en.cppreference.com/w/cpp/utility/functional/function

## Submission
Finally, you should submit EX3 (all three parts) using a single repository `ex3`.
Therefore you repo should look like this after EX3.2:
```
IteratorTests/*
SmartPtrTests/*     (new from EX3.2)
.gitignore
CMakeLists.txt      (updated: integrate build option for EX3.2 tests)
fibonacci_iterator.cpp
list.hpp
runTests.sh
unique_ptr.hpp      (new from EX3.2)
runSmartPtrTests.sh (new from EX3.2)
```

And you should integrate the build option for the new tests in your root CMakeLists.txt
```
...
option(BUILD_SMARTPTR_TESTS "Build unit tests to check the smart pointer functionality." ON)
if(BUILD_SMARTPTR_TESTS)
  add_subdirectory("SmartPtrTests")
endif()
...
```