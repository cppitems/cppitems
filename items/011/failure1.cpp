#include <utility>

struct Widget {
  int m;
//   Widget &operator=(Widget &other) = delete; // option forcing an additional error
};

// example for an error after substitution in the "body"

template <typename T> void swap(T &a, T &b) {
  using D = typename T::nonexisting; // (1) error when evaluating this expression after substituting T 
  T tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

int main() {
  Widget a{1};
  Widget b{2};
  swap(a, b); // error: pointing to (1)
}