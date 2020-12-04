1 // item status
# Exercise EX3.1

This exercise is part **one** of three parts of EX3. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX3 (all three parts) is **Mo	11.01.2021, 4pm**. The sources related to EX3.1 are available at [Item 017](https://github.com/cppitems/cppitems/tree/master/items/017).

## Task description

In this exercise you have to implement an STL conformal, bidirectional iterator for the provided class template `list`, which is a doubly-linked list (with only some of the functionality of an std::list implemented for simplicity).

This exercise is designed to mimic the workflow of software development in a professional setting. You are provided with several [unit tests](https://en.wikipedia.org/wiki/Unit_testing), which are very short programs laying out the required functionality. These tests are stored in the folder `IteratorTests` and we have set up CMake to automatically build them. Writing these tests is usually the first step in developement and they should lay out the wanted functionality of a part of software. Oftentimes the tests are written by one programmer and then satisfied by another to avoid an implicit bias by a single programmer. Therefore, we have provided them for you. They are ordered by increasing difficulty, so you should try to make one test work after the other. After running CMake in your build folder, you can build all tests using:

```bash
make
```

or just one test using the name of the test:

```bash
make TestA_dereference
```

In order to test the run-time validity of your iterator, you should run these tests as soon as they compile. Once all of them compile, you can run them all after each other with the provided shell script `runTests.sh`. If you implemented your iterator correctly, they should all execute successfully.

The project also includes a source file `fibonacci_iterator.cpp`, which should mimic the final executable we aim to develop. It is not built by default, but this can be done by using the cmake option to build it:

```bash
cmake -DBUILD_FIBONACCI_EXE=ON ..
```

## References
- Member Types needed for STL compatibility: https://en.cppreference.com/w/cpp/iterator/iterator_traits

## Submission
Finally, you should submit EX3 (all three parts) using a single repository `ex3`.
Therefore you repo should look like this after EX3.1:
```
IteratorTests/*
.gitignore
CMakeLists.txt
fibonacci_iterator.cpp
list.hpp
runTests.sh
```
