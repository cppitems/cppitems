#include <memory>

struct Base {
  virtual int calculate() = 0; // pure virtual
  virtual ~Base() = default;
};
struct Widget : public Base {
  int calculate() override {
    return 1;
  } // override requires somewhere in hierarcy
};

int main() {
  Base *base = new Widget{};                // upcasting
  Widget *w = dynamic_cast<Widget *>(base); // downcasting (for polymorphic)
  delete base;
}