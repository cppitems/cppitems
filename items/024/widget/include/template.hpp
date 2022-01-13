#pragma once
#include <iostream>

namespace widget::internal { 

template <typename T> struct Helper {
  T *data;
  Helper() : data(new T()) {}
  ~Helper() { delete data; }
  Helper &operator++() {
    ++(*data);
#ifdef DEBUGGING
    std::cout << "Debugging:" << *data << std::endl;
#endif

    return *this;
  }
};

} // namespace widget::internal