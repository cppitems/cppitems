#include <utility>

// modifications of swap

struct Widget {
  int m;
};

struct Widget2 {
  int m;
};

// non-template
void swap(Widget &a, Widget &b) {
  Widget tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

// template swapping .m directly and requiring a member m
template <typename T, typename D = decltype(T().m) > void swap(T &a, T &b) {
  D tmp(std::move(a.m));
  a.m = std::move(b.m);
  b.m = std::move(tmp);
};

// original template 
// template <typename T> void swap(T &a, T &b) {
//   T tmp(std::move(a));
//   a = std::move(b);
//   b = std::move(tmp);
// };


int func() {
  {
    Widget a{1};
    Widget b{2};
    swap(a, b);
  }
  {
    Widget2 a{1};
    Widget2 b{2};
    swap(a, b);
  }

}