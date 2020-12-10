struct Base {
  int b;
};
struct Widget : Base { // hierarchy
  int w;
};
struct WidgetFlat { // flat
  Base base;
  int w;
};

void func(Base &base) { base.b = 7; }

int main() {
  Widget w{};
  func(w); // hierarchy: implicit upcasting
  WidgetFlat w2{};
  func(w2.base); // flat: "manual" member passing
}