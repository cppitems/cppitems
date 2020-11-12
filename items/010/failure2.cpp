#include <utility>

struct Widget {
  int m;
};

template <typename T, typename D = typename T::nonexisting>
void swap(T &a, T &b) {
  T tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

int main() {
  Widget a{1};
  Widget b{2};
  swap(a, b);
}