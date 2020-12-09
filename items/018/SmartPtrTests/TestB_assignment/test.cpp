// testing assignment

#include "../../Widget.h"
#include "../../unique_ptr.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

int main() {
  std::cout << "TestB_assignment: ";

  { // check disabled copy assignment and construction
    auto up1 = unique_ptr(new Widget{});
    auto up2 = unique_ptr(new Widget{});
    static_assert(!std::is_copy_constructible<decltype(
                      up1)>::value); // auto up1 = unique_ptr(up2);
    static_assert(!std::is_copy_assignable<decltype(up1)>::value); // up1 = up2;
  }

  { // move assignment
    auto up1 = unique_ptr(new Widget{});
    auto up2 = unique_ptr(new Widget{});
    auto *ptr2 = up2.get();
    up1 = std::move(up2);
    assert(up1 != nullptr);
    assert(up2 == nullptr);
    assert(ptr2 == up1.get());
  }

  { // assign nullptr
    auto up = unique_ptr(new Widget{});
    up = nullptr;
    assert(up.get() == nullptr);
  }

  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}