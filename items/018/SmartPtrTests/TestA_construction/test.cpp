// testing construction

#include "../../Widget.h"
#include "../../unique_ptr.hpp"
#include <cassert>
#include <iostream>

int main() {
  std::cout << "TestA_construction: ";

  { // constructors
    // from pointer
    auto up1 = unique_ptr(new Widget{});
    auto *ptr1 = up1.get();
    // move constructions
    auto up2 = unique_ptr(std::move(up1));
    assert(up1.get() == nullptr); // error: "moved-from object not pointing to nullptr"
    assert(ptr1 == up2.get()); // error "move-destination not pointing to moved-from resource"
  }

  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}