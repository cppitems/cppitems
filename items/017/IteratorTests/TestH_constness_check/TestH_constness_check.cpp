/*
  This test checks, whether the iterators returned by begin() and cbegin() have the expected const or non-const access
*/

#include "../../list.hpp"
#include <iostream>

int main() {
  std::cout << "TestH_constness_check: ";

  // list::iterator and list::const_iterator have to be defined and default constructible
  {
    typename list<int>::iterator it;
    typename list<int>::const_iterator cit;

    *it;
    *cit;
  }

  // mutable list
  {
    list<int> testList(10, 42);

    // the type returned from begin() and end() of a non-const list should not be const
    static_assert(!std::is_const<decltype(testList.begin())::value_type>::value, "mutable.begin() should not be const!");
    static_assert(!std::is_const<decltype(testList.end())::value_type>::value, "mutable.end() should not be const!");

    // the type returned from cbegin() and cend() of a non-const list should be const
    static_assert(std::is_const<decltype(testList.cbegin())::value_type>::value, "mutable.cbegin() should be const!");
    static_assert(std::is_const<decltype(testList.cend())::value_type>::value, "mutable.cend() should be const!");
    {
      auto it = testList.begin();
      ++it;
      // should be mutable
      *it = 1234;
    }

    if(*(++testList.begin()) == 1234) {
      std::cout << "[ SUCCESS ]" << std::endl;
    } else {
      std::cout << "[ FAILED ]" << std::endl;
    }
  }

  // const list
  {
    const list<int> testList(10, 42);

    // the type returned from begin() and end() of a const list should be const
    static_assert(std::is_const<decltype(testList.begin())::value_type>::value, "const.begin() shoudl be const!");
    static_assert(std::is_const<decltype(testList.end())::value_type>::value, "const.end() should be const!");

    // the type returned from cbegin() and cend() of a const list should be const
    static_assert(std::is_const<decltype(testList.cbegin())::value_type>::value, "const.cbegin() should be const!");
    static_assert(std::is_const<decltype(testList.cend())::value_type>::value, "const.cend() should be const!");
  }

  return 0;
}