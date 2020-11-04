#include <functional>
#include <iostream>
struct Widget {
  int i;
};

int main() {
  using FuncPtr = Widget (*)(Widget &&);
  { // lambda

    FuncPtr fptr = [](Widget &&w) {
      ++(w.i);
      return w;
    };
    auto w = fptr(Widget{});
  }
  { // mimic lambda
    class ClosureType {
    private:
      static auto invoke(Widget &&w) {
        ++(w.i);
        return w;
      }

    public:
      operator FuncPtr() { return invoke; }
      auto operator()(Widget &&w) {
        ++(w.i);
        return w;
      } // (2)
    };
    FuncPtr fptr = ClosureType();
    auto w = fptr(Widget{});
  }
}