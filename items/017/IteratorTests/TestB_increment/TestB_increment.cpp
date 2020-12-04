/*
  This test checks, whether the function list.begin() returns an incrementable iterator
*/

#include "../../list.hpp"
#include <iostream>

int main() {
  std::cout << "TestB_increment: ";
  list<int> testList;
  testList.push_back(0);
  testList.push_back(1);

  // try incrementing
  auto it = testList.begin();

  ++it;

  if(*it == 1) {
    std::cout << "[ SUCCESS ]" << std::endl;
  } else {
    std::cout << "[ FAILED ]" << std::endl;
  }

  return 0;
}