#include <utility>

struct Widget {
  int m;
};

// example for failures after substitution in expressions related to the signature of the function

// (1) failure when evaluating the default expression for the D template parameter
template <typename T,typename D = typename T::nonexisting>
void swap(T &a, T &b) {
  T tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

// (2) failure when evaluating the default expression for the D function argument
template <typename T>
void swap(T &a, T &b, decltype(T::nonexisting) D = decltype(T::nonexisting)()) {
  T tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

// (3) this will be used
template <typename T>
void swap(T &a, T &b) {
  T tmp(std::move(a));
  a = std::move(b);
  b = std::move(tmp);
};

int main() {
  Widget a{1};
  Widget b{2};
  swap(a, b); // (3) will be used, (1) and (2) are ignored but do not fail the compilation
}