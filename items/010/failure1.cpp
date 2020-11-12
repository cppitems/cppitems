#include <utility>

struct Widget {
  int m;
//   Widget &operator=(Widget &other) = delete;
};

template <typename T> void swap(T &a, T &b) {
  using D = typename T::nonexisting;
  T tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

int main() {
  Widget a{1};
  Widget b{2};
  swap(a, b);
}