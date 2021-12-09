struct Base {
  int b;
  void member(){};
};
struct Widget : Base {
  int w;
  void member(){};
};
struct Other : Widget {
  int o;
  void member(){};
};
// // footprint of layout
// struct OtherFootprint {
//   int b; // from Base
//   int w; // from Widget
//   int o;
// };

int main() {
  Other o{{{5}, 7}, 6};
//   OtherFootprint o{5, 7, 6};
  o.Base::b = 5;
  o.Widget::w = 6;
  o.Other::o = 7;
  o.Other::member();
  o.Base::member();
  o.Widget::member();
}