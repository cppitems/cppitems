/*
  This test checks, whether the function list.end() returns a decrementable iterator
*/

#include "../../list.hpp"
#include <iostream>

int main() {
  std::cout << "TestD_decrement_end: ";
  list<int> testList;
  testList.push_back(0);
  testList.push_back(1);
  testList.push_back(2);

  // try incrementing
  auto it = testList.end();

  // decrementing should resolve to last element of list
  --it;

  if(*it == 2) {
    std::cout << "[ SUCCESS ]" << std::endl;
  } else {
    std::cout << "[ FAILED ]" << std::endl;
  }

  return 0;
}