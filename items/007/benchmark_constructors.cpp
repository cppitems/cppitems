#include "vector.hpp"

#include <chrono>
#include <iostream>

using VectorD = Vector<double>;

struct Widget {
  VectorD vec;
  Widget(size_t count, const VectorD::value_type &value) : vec(count, value) {}
};

// 'main' measures the runtime for
// move constructor (testMove) and
// copy constructor (testCopy)
int main() {

  size_t size = 1'000'000; // vector length 1M == ~7MB
  size_t n = 10;           // iterations for averaging runtime

  auto testMove = [size]() {
    Widget var = Widget(size, std::rand());
    const VectorD::value_type *tmp = var.vec.data();
    Widget w = std::move(var); // invoking move constructiont
    if (var.vec.data() != nullptr || w.vec.data() != tmp) {
      std::cout << "Widget was not move constructed, this can be improved!"
                << std::endl;
    }
  };
  auto testCopy = [size]() {
    Widget var = Widget(size, std::rand());
    Widget w = var; // invoking copy construction
    if (w.vec.data() == nullptr) {
      std::cout << "Widget was moved from lvalue, should not happen!"
                << std::endl;
    }
    if (var.vec.size() != w.vec.size()) {
      std::cout << "vectors of not equal size or capacity, should not happen!"
                << std::endl;
    }
    for (unsigned i = 0; i < var.vec.size(); ++i) {
      if (var.vec[i] != w.vec[i]) {
        std::cout << "Vector elements " << i
                  << " are not the same, should not happen!" << std::endl;
      }
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

  auto timeMove = runtest(testMove);
  auto timeCopy = runtest(testCopy);
  std::cout << std::scientific;
  std::cout << "Moving took:  " << timeMove << " s" << std::endl;
  std::cout << "Copying took: " << timeCopy << " s" << std::endl;
  std::cout << std::fixed << "Moving was " << (timeCopy / timeMove)
            << " times faster!" << std::endl;
}