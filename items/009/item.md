1 // item status
# Exercise EX1.2

The exercise is available as a git repository at:
https://github.com/cppitems/ex1.2

# Possible solution
```pmans
#include <algorithm> // access to std::fill, std::copy, std::swap

// Note: iterator based algorithms from stdlib also work with 'raw' pointers

// copy constructor
Vector::Vector(const Vector &other)
    : _size(other._size), _data(/*b3*/ new Vector::value_type[_size]) {
  const auto /*f*/ srcBegin /*x*/ = other._data; 
  const auto /*f*/ srcEnd /*x*/ = other._data + other._size; 
  auto /*f*/ destBegin /*x*/ = _data; 
  std::copy(/*f*/ srcBegin /*x*/, /*f*/ srcEnd /*x*/, /*f*/ destBegin /*x*/);
}

// move constructor
Vector::Vector(Vector &&other) {
  // Note: a generic way is to 'swap on move'
  // Important: have default initializers for the members
  //  to have a valid state of 'this' before swapping
  std::swap(other._data, _data);
  std::swap(other._size, _size); //
}

// destructor
Vector::~Vector() {
  /*b8*/ delete[] _data; // release resources the object owns
  _size = 0; // doing this is ok, but then also ...
  _data = nullptr; // ... this should be done ...
  // ... but both 'resets' are not required:
  // 1. destructor is usually only called at the end of an object's lifetime 
  // 2. an object should not be used again after destruction 
  // 3. do not use a destructor explicitly for 'resetting'
  // 4. instead use a separate function to 'reset' data structure
}
```

## Benchmark results
```bash
Copy:
Old data before  | New data after   | Old Data after
  0x55b947d97bc0 |   0x55b948538dd0 | 0x55b947d97bc0

Move:
Old data before  | New data after   | Old Data after
  0x55b947d97bc0 |   0x55b947d97bc0 | 0


Runtimes of constructing vector objects:

Copy constructor Vector(const Vector&)  called as Vector(vec)             1.60e-03 s -> alloc + copy
Move constructor Vector(Vector&&)       called as Vector(std::move(vec))  6.48e-04 s -> only swapping 
```