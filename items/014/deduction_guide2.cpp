#include <list>
#include <string>
#include <vector>

struct Other {
  using value_type = int;
  value_type m;
};

template <typename T> struct Widget {
  T m;
  template <typename OTHER>
  Widget(const OTHER &o) {} // construct Widget from OTHER (unrelated) type
};
// user-provided deduction guide
template <typename OTHER>
Widget(const OTHER &o) -> Widget<typename OTHER::value_type>;

int main() {
  Widget w(Other{});
}
