// testing release and reset of resource

#include "../../Widget.h"
#include "../../unique_ptr.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

int main() {
  std::cout << "TestC_release_reset: ";

  { // reset of resource
    auto up1 = unique_ptr(new Widget{});
    auto *ptr = new Widget{};
    up1.reset(ptr);
    assert(up1.get() == ptr); // error: "reset(ptr) not resetting resource to ptr"
    up1.reset();
    assert(up1.get() == nullptr); // error: "reset() not resrtting to nullptr" 
  }

  { // release of resource
    Widget *ptr1 = new Widget{};
    Widget *ptr2 = nullptr;
    {
      auto up = unique_ptr(ptr1);
      ptr2 = up.release();
      assert(up.get() == nullptr); // error: "release() not resetting to nullptr"
      assert(ptr1 == ptr2); // error: "release() not returning previously managed resource"
    }
    auto up = unique_ptr(ptr2); // for cleanup only
  }

  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}