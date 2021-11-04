#include <iostream>
#include <utility>

auto func(int a, double b, float c) {
  std::cout << a << ',' << b << ',' << c << std::endl;
  return a + b + c;
}

template <typename... ARGS> auto sum_args(ARGS &&... args) {
  return (0 + ... + args);
};
template <typename... ARGS> auto print_args(ARGS &&... args) {
  ((std::cout << args << ','), ..., (std::cout << std::endl));
};
template <typename... ARGS> auto mult_args(ARGS &&... args) {
  return (1 * ... * args);
};
template <typename... ARGS> auto forward_args(ARGS &&... args) {
  return func(args...);
};
template <typename... ARGS> auto perfect_forward_args(ARGS &&... args) {
  return func(std::forward<ARGS>(args)...);
};
template <typename FUNC, typename... ARGS>
auto perfect_function_wrapper(FUNC &&callable, ARGS &&... args) {
  return callable(std::forward<ARGS>(args)...);
};

int main() {
  print_args(1, 2, 3);
  std::cout << sum_args(1, 2, 3) << std::endl;
  std::cout << mult_args(1, 2, 3) << std::endl;
  std::cout << forward_args(1, 2, 3) << std::endl;
  std::cout << perfect_forward_args(1, 2, 3) << std::endl;
  std::cout << perfect_function_wrapper(func, 1, 2, 3) << std::endl;
}