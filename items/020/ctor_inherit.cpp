struct Base {
  int b;
  Base(int b) : b(b) {}
  Base(int a, int b) : b(a + b) {}
  Base(int a, int b, int c) : b(a + b * c) {}
};
struct Widget : Base {
  int w = 1;
  using Base::Base; // inherit all Base constructors
};

int main() {
  Widget w(5, 6);
  Widget w2(5, 6, 7);
}