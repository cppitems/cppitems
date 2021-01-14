#include "widget.hpp"
#include "template.hpp"

namespace widget {

  using namespace widget::internal;

  struct Widget::impl {
    Helper<int> m;
  };

  Widget::Widget() : ptr(std::make_shared<Widget::impl>()) {}
  Widget &Widget::operator++() {
    ++(ptr->m);
    return *this;
  }

}