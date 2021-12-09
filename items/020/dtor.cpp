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
  // destruction sequence for 'w':
  // w.~Widget()
  //   w.~w()
  // w.Base::~Base()
  //   w.Base::b()
}