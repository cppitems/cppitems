#pragma once
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

template <typename T> class unique_ptr {
  T *ptr;

public:
  // construction
  unique_ptr(T *p) : ptr(p) {}

  // destruction
  ~unique_ptr() { delete ptr; }

  // access
  T *get() { return ptr; }
  T &operator*() { return *get(); }
  T *operator->() { return get(); }
};
