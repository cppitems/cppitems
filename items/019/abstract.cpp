#include <memory>

struct Base {
  virtual int calculate() = 0; // pure virtual
  virtual ~Base() = default;
};
struct Widget : public Base {
  int calculate() override { return 1; } // override requires somewhere in hierarcy
};
struct Widget2 : public Widget {
//   int calculate() override { return 1; } // override requires somewhere in hierarcy
};

int main() {
    auto up = std::unique_ptr<Base>(new Widget2{});   
}