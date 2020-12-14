// testing comparisons

#include "../../Widget.h"
#include "../../unique_ptr.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

int main() {
  std::cout << "TestD_compare: ";

  { // compare to nullptr
    auto up = unique_ptr(new Widget{});
    assert(up != nullptr); // error: "compare unequal to nullptr not working as expected"
    up = nullptr;
    assert(up == nullptr); // error: "compare equal to nullptr not working as expected"
  }

  { // compare to same type
    auto up1 = unique_ptr(new Widget{});
    auto up2 = unique_ptr(new Widget{});
    assert(up1 != up2); // error: "compare unequal to other unique_ptr not working as expected"
    auto up3 = unique_ptr(up2.get());
    assert(up2 == up3); // error: "compare equal to other unique_ptr not working as expected"
    up3.release();
  }


  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}