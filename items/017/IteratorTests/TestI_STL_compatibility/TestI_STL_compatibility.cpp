/*
  This test checks, whether the iterators returned are STL conformal and can be used with STL functions. Member types required for STL conformality: https://en.cppreference.com/w/cpp/iterator/iterator_traits
*/

#include "../../list.hpp"
#include <iostream>
#include <algorithm>

int main() {
  std::cout << "TestI_STL_compatibility: ";

  list<int> testList;

  // fill list with values
  for(int i = 0; i < 8; ++i) {
    testList.push_back(i);
  }

  // move iterator 6 times
  auto it = testList.begin();
  for(unsigned i = 0; i < 6; ++i) {
    ++it;
  }

  // use std::distance to test STL compatibility
  auto index = std::distance(testList.begin(), it);

  // length of whole list
  list<int>::size_type length = std::distance(testList.begin(), testList.end());

  if(index == 6 && length == testList.size()) {
    std::cout << "[ SUCCESS ]" << std::endl;
  } else {
    std::cout << "[ FAILED ]" << std::endl;
  }

  return 0;
}