#include <utility>

template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename AUTO> struct Widget { Widget(AUTO &&arg){}; };

int main() {
  double lval = 1.0;
  double &lref = lval;
  Widget w1(lref);
  Widget w2(std::move(std::as_const(lref)));
  Widget w3(std::as_const(lref));
  Widget w4(std::move(lref));
  Widget w5(1.0);
}