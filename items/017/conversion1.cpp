struct Widget1 {
  int m;
};

struct Widget2 {
  int m;
  Widget2() : m(){};
  // (1) converting constructor from 'Widget' to 'Widget2'
  explicit Widget2(const Widget1 &w) : m(w.m){};
  // (2) conversion function from 'Widget2' to 'Widget'
  explicit operator Widget1() { return Widget1{m}; };
};

int main() {
  Widget1 w1;
  Widget2 w2;
  w2 = w1; // implicit conversion using (1): error if (1) is specified if 'explicit'
  w2 = static_cast<Widget2>(w1);  // explicit conversion using (1)
  w1 = w2; // implicit conversion function (2): error if (2) is specified if 'explicit'
  w1 = static_cast<Widget1>(w2);  // explicit conversion using (2)
}
