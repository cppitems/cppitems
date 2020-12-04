/*
  This test checks, whether the function list.begin() returns a dereferencable iterator
*/

#include "../../list.hpp"
#include <iostream>

int main() {
  std::cout << "TestA_dereference: ";
  list<double> testList(10, 3.1415);

  auto it = testList.begin(); // move-assign

  if(*it == 3.1415) {
    std::cout << "[ SUCCESS ]" << std::endl;
  } else {
    std::cout << "[ FAILED ]" << std::endl;
  }

  return 0;
}