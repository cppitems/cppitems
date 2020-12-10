struct Base {
  int b;
  ~Base() {}
};
struct Widget : Base {
  int w;
  ~Widget() {}
};

int main() {
  Widget w{};
  // destruction sequence:
  // w.~Widget()
  // w.~w
  // w.Base::~Base()
  // w.Base::b
}