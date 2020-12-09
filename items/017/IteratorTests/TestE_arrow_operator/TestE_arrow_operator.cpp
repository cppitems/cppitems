/*
  This test checks, whether the iterator has an arrow (->) operator
*/

#include "../../list.hpp"
#include <iostream>

// data structure with member, that can be accessed with
// the arrow operator later
struct testClass {
  int member;
};

int main() {
  std::cout << "TestE_arrow_operator: ";

  // list of structure with member
  list<testClass> testList;

  for(int i = 0; i < 5; ++i) {
    testList.push_back(testClass{i});
  }

  // check whether member can be retrieved with arrow operator
  unsigned counter = 0;
  for(auto it = testList.begin(); it != testList.end(); ++it) {
    if(it->member != static_cast<int>(counter)) {
      std::cout << "[ FAILED ]: expected " << counter << ", but got " << it->member << std::endl;
      return 1;
    }
    ++counter;
  }

  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}