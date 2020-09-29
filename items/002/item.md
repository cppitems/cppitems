1 // item status
# Exercise EX0

## 1. Make sure you have a working environment configured with access to
- git 
- cmake >= 3.0
- compiler/linker for C++17
- recommended: clangd (clang-format, clang-tidy)

### You can use the preconfigured docker-container (Theia-IDE + clang/LLVM) provided here 
https://github.com/cppitems/docker

> What is a 'docker-container'? 

> What is the 'Theia-IDE'?

## 2. Coding task

You can find a C++ project with a CMake configuration at

https://github.com/cppitems/cppitems/tree/master/items/002/ex0

Your task is to add the implementation of a `Vector` class to this project so that it compiles with the usage shipped in `main.cpp`, which also contains further details and submission modalities.

```pmans
int main() {

  auto vec = /*b*/ Vector(1000, 10) /*x*/; // ctor of length '1000', init with '10'

  {                                         // check size
    /*b*/ Vector::size_type /*x*/ size = /*b*/ vec.size() /*x*/; // nested type
    if (size != 1000) {
      std::cout << "size not 1000" << std::endl;
    }
  }

  { // sweep over vector and write
    for (size_t i = 0; i != vec.size(); ++i) {
      /*b*/ vec.at(i) /*x*/ = /*b*/ Vector::value_type(1) /*x*/;
    }
    if (/*b*/ vec.sum() /*x*/ != 1000) { // sum and check
      std::cout << "sum not 1000" << std::endl;
    }
  }

  { // access raw data
    /*b*/ Vector::value_type * /*x*/data = /*b*/ vec.data() /*x*/;
    for (size_t i = 0; i != vec.size(); ++i) {
      data[i] = Vector::value_type(2);
    }
    if (vec.sum() != 2000) {
      std::cout << "sum not 2000" << std::endl;
    }
  }

  { // copy assignment
    /*b*/ Vector other = vec /*x*/;
    if (vec.data() == other.data()) {
      std::cout << "copy assignment: data should not alias" << std::endl;
    }
    if (vec.sum() != other.sum()) {
      std::cout << "copy assignment: sums dont match" << std::endl;
    }
  }
  { // copy ctor
    /*b*/ auto other = Vector(vec) /*x*/;
    if (vec.data() == other.data()) {
      std::cout << "copy ctor: data should not alias" << std::endl;
    }
    if (vec.sum() != other.sum()) {
      std::cout << "copy ctor: sums dont match" << std::endl;
    }
  }
  return 0;
}
```


