// testing custom deleters 

#include "../../Widget.h"
#include "../../unique_ptr.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

int count_deleter_function_calls = 0;
int count_deleter_lambda_calls = 0;

void deleter_function(Widget *ptr) {
  delete ptr;
  ++count_deleter_function_calls;
};

[[maybe_unused]] auto deleter_lambda = [](auto *ptr) {
  delete ptr;
  ++count_deleter_lambda_calls;
};

int main() {
  std::cout << "TestE_deleter: ";

  { // usind deleter function and deleter lambda
    auto up1 = unique_ptr(new Widget{}, deleter_function);
    auto up2 = unique_ptr(new Widget{}, deleter_lambda);
  }
  assert(count_deleter_function_calls == 1); // error: "expected one call to deleter_function"
  assert(count_deleter_lambda_calls == 1); // error: "expected one call to deleter_lambda"

  { // deleter function + move construction
    auto up1 = unique_ptr(new Widget{}, deleter_function);
    auto *ptr1 = up1.get();
    unique_ptr<Widget> up2 = std::move(up1);
    assert(up2 != nullptr); // error: "moved-to object is nullptr"
    assert(up1 == nullptr); // error: "moved-from object is not nullptr"
    assert(ptr1 == up2.get()); // error: "moved-to object is not the moved-from resource"
  }
  assert(count_deleter_function_calls == 2); // error: "expected two calls to deleter_function"
  assert(count_deleter_lambda_calls == 1);  // error: "expected one call to deleter_function"

  { // deleter lambda + move assignment
    auto up1 = unique_ptr(new Widget{});
    auto up2 = unique_ptr(new Widget{}, deleter_lambda);
    auto *ptr2 = up2.get();
    up1 = std::move(up2);
    assert(up1 != nullptr); // error: "moved-to object is nullptr"
    assert(up2 == nullptr); // error: "moved-from object is not nullptr"
    assert(ptr2 == up1.get()); // error: "moved-to object is not the moved-from resource"
  }
  assert(count_deleter_function_calls == 2); // error: "expected two calls to deleter_function"
  assert(count_deleter_lambda_calls == 2); // error: "expected two calls to deleter_function"

  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}