// 360.251 Exercise 'EX0':
// 1. implement a class 'Vector' in 'vector.h' (and 'vector.cpp' if
// you like) which complies with the following usage and passes the 'tests'
// (which are just console logs).
// 2. You are allowed to use the c++ standard library, but no other external
// libraries.
// 3. Use the provided CMakeLists.txt to configure your project (no
// changes should be needed there).
// 4. Submit your code by pushing it to the 'master' of your 'ex1' repository on
// https://tea.iue.tuwien.ac.at
// 5. The submission deadline is: Mo 15.10.2020, 4pm
// 6. For urgent questions contact us at cpp@iue.tuwien.ac.at

#include "vector.h"
#include <iostream>

int main() {

  auto vec = Vector(1000, 10); // ctor: length '1000', init with '10'

  {                                      // check size
    Vector::size_type size = vec.size(); // nested type
    if (size != 1000) {
      std::cout << "size not 1000" << std::endl;
    }
  }

  { // sweep over vector and write
    for (size_t i = 0; i != vec.size(); ++i) {
      vec.at(i) = Vector::value_type(1);
    }
    if (vec.sum() != 1000) { // sum and check
      std::cout << "sum not 1000" << std::endl;
    }
  }

  { // access raw data
    Vector::value_type *data = vec.data();
    for (size_t i = 0; i != vec.size(); ++i) {
      data[i] = Vector::value_type(2);
    }
    if (vec.sum() != 2000) {
      std::cout << "sum not 2000" << std::endl;
    }
  }

  { // copy assignment
    Vector other = vec;
    if (vec.data() == other.data()) {
      std::cout << "copy assignment: data should not alias" << std::endl;
    }
    if (vec.sum() != other.sum()) {
      std::cout << "copy assignment: sums dont match" << std::endl;
    }
  }
  { // copy constructor
    Vector other(vec);
    if (vec.data() == other.data()) {
      std::cout << "copy constructor: data should not alias" << std::endl;
    }
    if (vec.sum() != other.sum()) {
      std::cout << "copy constructor: sums dont match" << std::endl;
    }
  }
  return 0;
}