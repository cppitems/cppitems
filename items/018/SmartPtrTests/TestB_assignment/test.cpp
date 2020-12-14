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
                      up1)>::value); // error: "copy construction is not disabled"
    static_assert(!std::is_copy_assignable<decltype(up1)>::value); // error: "copy assignment is not disabled"
  }

  { // move assignment
    auto up1 = unique_ptr(new Widget{});
    auto up2 = unique_ptr(new Widget{});
    auto *ptr2 = up2.get();
    up1 = std::move(up2);
    assert(up1 != nullptr); // error: "move-assigned object points to nullptr"
    assert(up2 == nullptr); // error: "moved-from object does not point to nullptr"
    assert(ptr2 == up1.get()); // error: "move-assigned object does not point to moved-from object"
  }

  { // assign nullptr
    auto up = unique_ptr(new Widget{});
    up = nullptr;
    assert(up.get() == nullptr); // error: "object which was assigned using a nullptr does not point to nullptr"
  }

  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}