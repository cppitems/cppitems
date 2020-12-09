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
    assert(up != nullptr);
    up = nullptr;
    assert(up == nullptr);
  }

  { // compare to same type
    auto up1 = unique_ptr(new Widget{});
    auto up2 = unique_ptr(new Widget{});
    assert(up1 != up2);
    auto up3 = unique_ptr(up2.get());
    assert(up2 == up3);
    up3.release();
  }


  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}