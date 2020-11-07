#include <memory>
#include <vector>

// snippets used to discusses questions in the lecture:

struct Widget {
  int i;
};
using RrefType = Widget &&;
void take_lref(Widget &lref) {}
void take_rref(Widget &&rref) {}

void func(Widget &lref, Widget &&rref) {
  // how to use rref?
  // 'rref' is lvalue !
  // everything with a handle/name is treated lvalue
  take_lref(lref);
  take_rref(std::move(rref));
}

int main() {

  auto &ref1 = "doesnotwork"; // does not work
  const auto &ref2 = "works"; // works
}
