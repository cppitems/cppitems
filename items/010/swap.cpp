#include <utility>

// adoptions
// - second widget2 (add second template parameter)
// - explicitly specify incompatible parameter 
// - remove default for D
// - overload
// - check symbols using nm  (have second widget type)
// - template argument nested in the paramter type

struct Widget {
  int m;
};

template <typename T>
void swap(T &a, T &b) {
  T tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

// void swap(Widget &a, Widget &b) {
//   Widget tmp(std::move(a));
//   a = std::move(b);
//   b = std::move(tmp);
// };


// template <typename T, typename D = decltype(T().m)>
// void swap(T &a, T &b) {
//   T tmp(std::move(a));
//   a = std::move(b);
//   b = std::move(tmp);
// };

int func() {
  Widget a{1};
  Widget b{2};
  swap(a, b);
}