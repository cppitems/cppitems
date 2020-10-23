#include <chrono>
#include <iostream>
#include <vector>

using Vector = std::vector<double>;

struct Widget {
  Vector vec;
  Widget(size_t size, Vector::value_type init) : vec(size, init) {}
  Widget(const Widget &) = default; // implicit copy constructor
  Widget(Widget &&) = default;      // implicit move constructor
};

// 'main' measures the runtime for
// implicit move constructor (test1) and
// implicit copy constructor (test2)
int main() {

  size_t size = 1'000'000; // vector length 1M == ~7MB
  size_t n = 10;           // iterations for averaging runtime

  auto test1 = [size]() {
    Widget var = Widget(size, std::rand());
    Widget w = std::move(var); // invoking implicit move construction
    if (var.vec.data() != nullptr) {
      std::cout << "widget was not move constructed, this can be improved"
                << std::endl;
    }
  };
  auto test2 = [size]() {
    Widget var = Widget(size, std::rand());
    Widget w = var; // invoking implicit copy construction
    if (w.vec.data() == nullptr) {
      std::cout << "Widget was moved from lvalue, should not happen"
                << std::endl;
    }
    if (var.vec.size() != w.vec.size() || var.vec[1] != w.vec[1]) {
      std::cout << "vectors of not equal size, or different value, should not happen"
                << std::endl;
    }     
  };

  auto runtest = [n](auto &&test) {
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<double>;
    auto start = Clock::now();
    for (size_t i = 0; i < n; ++i) {
      test();
    }
    auto stop = Clock::now();
    return Duration(stop - start).count() / n;
  };

 std::cout << std::scientific;
  std::cout << runtest(test1) << "s" << std::endl;
  std::cout << runtest(test2) << "s" << std::endl;
}