#include <utility>

template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename /*f4*/ AUTO> struct Widget {
  Widget(/*b4*/ const AUTO &arg){  };
  Widget(/*b4*/ AUTO &&arg){};
};

int main() {
  double lval = 1.0;
  double &lref = lval;
  double *ptr = &lref;
  Widget w1(std::move(std::as_const(lref))); 
  Widget w2(std::as_const(lref)); 
  Widget w3(std::move(lref)); 
  Widget w4(1.0); 
  Widget w5(ptr); 
  Widget w6(&lval); 
}