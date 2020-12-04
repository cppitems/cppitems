/*
  This test checks, whether the function list.begin() returns a decrementable iterator
*/

#include "../../list.hpp"
#include <iostream>

int main() {
  std::cout << "TestC_decrement: ";
  list<int> testList;
  testList.push_back(0);
  testList.push_back(1);

  // try incrementing
  auto it = testList.begin();

  ++it;
  --it;

  if(*it == 0) {
    std::cout << "[ SUCCESS ]" << std::endl;
  } else {
    std::cout << "[ FAILED ]" << std::endl;
  }

  return 0;
}