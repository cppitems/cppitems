/*
  This test checks, whether the iterator can be used in a range-based for loop
*/

#include "../../list.hpp"
#include <iostream>

int main() {
  std::cout << "TestG_range_based_loop: ";
  list<int> testList;

  // fill list with values
  int values[8] = {-12, -324, 0, 1, 2, 42, 114, 2048};

  for(unsigned i = 0; i < 8; ++i) {
    testList.push_back(values[i]);
  }
  
  unsigned counter = 0;
  for(auto &it : testList) {
    if(counter > 7) {
      std::cout << "[ FAILED ]: outside of range" << std::endl;
      return 1;
    }
    if(it != values[counter]) {
      std::cout << "[ FAILED ]: wrong value: " << it << " instead of " << values[counter] << std::endl;
      return 1;
    }
    ++counter;
  }

  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}