#include <utility>

template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename AUTO> struct Widget { Widget(AUTO &arg){}; };

int main() {
  double lval = 1.0;
  double &lref = lval;
//   Widget w1(lval);
//   Widget w2(lref);
//   Widget w3(std::as_const(lref));
//   DebugType<decltype(w3)> error;
//   Widget w4(std::move(lref));
//   Widget w5(1.0);
}