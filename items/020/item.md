1 // item status
# Exercise EX3.3

This exercise is part **three** of three parts of EX3. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX3 (all three parts) is **Mo	11.01.2021, 4pm**. The sources related to EX3.3 are available at [Item 020](https://github.com/cppitems/cppitems/tree/master/items/020).

## Task description

The idea of this exercise is to showcase two approaches to design a library interface where the user provides an implementation of a required functionality which is then used by the library:

- the user has to derive from a non-templated virtual base class 
  - the interface requirements can be enforced using pure virtual functions
  - the library and the user code can be compiled in separate compilation units
- the user has to provide a type which is passed as template parameter to the (header-only) library
  - the interface requirements can be captured using `static_assert` + type traits
  - the user code and the library have to be compiled in a single translation unit 

In this exercise, two full implementations of a small library are provided which represent the above approaches. The library approximates the value of Pi using pseudo random numbers sampling the unit square. 
The user has to provide a type (called *generator* in the following) which yields a double precision random number between 0 and 1 when invoked without arguments using the `operator()`.


### Task 1: user provided generator class
It is your task to take the perspective of a user and complete the implementation of two *generators* (in `generators.hpp`) which are compatible with the library interfaces and the usage in the provided tests in `polymorphism.cpp`.

The starting point (which is provided) looks like this for the approach using *run time polymorphism*:
```pmans
/* file: generators.hpp */ 
  template <class EngineType> class Generator : public GeneratorBase {
  public:
    /*f*/ Generator(typename EngineType::result_type seed) /*x*/ {}
    /*f*/ double operator()() override /*x*/ { return 0; } 
  };
```
The base class `GeneratorBase` which prescribes the formal interface is found in `librandompi/RandomPi_library.hpp`.
For the header-only *compile time polymorphism* approach the class looks very similar, just that there is not inheritance:
```pmans
/* file: generators.hpp */ 
  template <class EngineType> class Generator {

  public:
    /*f*/ Generator(typename EngineType::result_type seed) /*x*/ {}
    /*f*/ double operator()() /*x*/ { return 0; }
  };
```
The formal interface is enforced at compile type using `static_assert`s  in `librandompi/RandomPi_headeronly.hpp`.

It is your task to complete/adopt the mock-up implementations such that the classes make use of the predefined random number generators from the stdlib (in `<random>`) to draw the random numbers if they are constructed like this for the tests:
```pmans
/* file: polymorphism.cpp */
auto gen = std::make_shared<headeronly::Generator</*f*/ std::minstd_rand /*x*/>>(seed); // basic generator
auto gen = std::make_shared<headeronly::Generator</*f*/ std::mt19937 /*x*/>>(seed); // mersenne twister generator
```
Further it is required that the  `operator()` yields a random number in the interval [0, 1), e.g. using `std::uniform_real_distribution` from `<random>`.

Note: you are allowed to adopt the classes as you need, as long as they compile with the provided tests in `polymorphism.cpp`.

### Task 2: Performance observations

In this second task you have to collect some performance results subject to the following variations:
- Debug vs Release build 
  - Note: use CMake to change this setting
- *virtual* vs *pure virtual* function for the `operator()` 
  - Note: edit `librandompi/RandomPi_library.hpp` to change this
- run time vs compile time polymorphism 
  - Note: the tests in `polymorphism.cpp` already measure both

It is recommended to use the tests provided in `polymorphism.cpp` (which is compiled to the `testPolymorphism` executable) to fill the following table for a fixed number of random samples (e.g. 5e7) when

- using `std::minstd_rand`:

|  | virtual  | pure virtual  | header-only  | 
|---|---|---|---|
| **Debug**  | runtime?  | runtime?  |  runtime? |
| **Release**  | runtime?  | runtime?  | runtime?  |

- and when using `std::mt19937`:

|  | virtual  | pure virtual  | header-only  | 
|---|---|---|---|
| **Debug**  | runtime?  | runtime?  |  runtime? |
| **Release**  | runtime?  | runtime?  | runtime?  |

**Note**: It is not required to submit your performance results but you have to be able to report your results qualitatively and discuss them in the discussion sessions.

## Background: estimating the value of Pi

The value of Pi can be approximated considering the area of a circle:

```
A = pi * r^2 = pi (for the unit circle r = 1)
```

If we consider only the first quadrant of the unit circle, as shown
in the figure below, it is bound to the intervals x = [0, 1] and
y = [1, 0].
The area of the first quadrant is simply `1`, since it is a square
of edge length 1. The area of the circle inside the first quadrant is just a quarter
of its entire area:

```
A_1 = pi / 4
```

If we create N random coordinates in the first quadrant and record 
the number of random coordinates inside the circle (N_inside), 
we can estimate the value of Pi:

```
A_1 = N_inside / N = pi / 4

--> pi = 4 * N_inside / N
```

![unit_circle](https://github.com/cppitems/cppitems/raw/master/items/020/unit_circle.png)

Fig.1: First quadrant of the unit circle, shown as a dashed grey line. It is used to generate random coordinates a and b. Only a is inside the circle, so using only those two points, our estimate of Pi is 4 * 1/2 = 2 .

## Submission

Finally, you should submit EX3 (all three parts) using a single repository `ex3`.
Therefore you repo should look like this after EX3.3:

```
IteratorTests/*
SmartPtrTests/*
librandompi/CMakeLists.txt
librandompi/RNGenerator.hpp          (new)
librandompi/RandomPi_library.hpp     (new)
librandompi/RandomPi_library.hpp     (new)
libRandomPi/RandomPi_headeronly.cpp  (new)
generators.hpp                       (new)
polymorphism.cpp                     (new)
.gitignore
CMakeLists.txt                       (updated: integrate build for EX3.3)
fibonacci_iterator.cpp
list.hpp
runTests.sh
unique_ptr.hpp
runSmartPtrTests.sh
```
And you should integrate the necessary CMake commands in your root CMakeLists.txt:
```
...
# BUILD LIB
add_subdirectory(librandompi)
# BUILD EXECUTABLE
add_executable(testPolymorphism polymorphism.cpp)
target_link_libraries(testPolymorphism librandompi)
...
```
