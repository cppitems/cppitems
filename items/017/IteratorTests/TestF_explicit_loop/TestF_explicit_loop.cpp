/*
  This test checks, whether the iterator can be used in an explicit loop
*/

#include "../../list.hpp"
#include <iostream>

int main() {
  std::cout << "TestF_explicit_loop: ";
  list<int> testList;

  // fill list with values
  int values[8] = {-12, -324, 0, 1, 2, 42, 114, 2048};

  for(unsigned i = 0; i < 8; ++i) {
    testList.push_back(values[i]);
  }
  
  unsigned counter = 0;
  for(typename list<int>::iterator it = testList.begin(); it != testList.end(); ++it) {
    if(counter > 7) {
      std::cout << "[ FAILED ]: outside of range" << std::endl;
      return 1;
    }
    if(*it != values[counter]) {
      std::cout << "[ FAILED ]: wrong value: " << *it << " instead of " << values[counter] << std::endl;
      return 1;
    }
    ++counter;
  }

  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}