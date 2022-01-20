#pragma once
#include <memory>

namespace widget {

struct Widget {
  struct impl;
  std::shared_ptr<impl> ptr;
  Widget();
  Widget& operator++();
};

}