#include <chrono>
#include <iostream>
#include <vector>

using Vector = std::vector<double>;

struct Widget {
  Vector vec;
};

// implement a free function initializing a Widget using
// a rvalue reference to a 'std::vector' as argument
Widget init(Vector &&rref){
    // YOUR IMPLEMENTATION STARTS HERE
    return Widget{}; // adopt line this as needed;
    // YOUR IMPLEMENTATION STOPS HERE
};

// implement a free function initializing a Widget using
// a const lvalue reference to a 'std::vector' as argument
Widget init(const Vector &lref){
    // YOUR IMPLEMENTATION GOES HERE
    return Widget{}; // adopt this line as needed;
    // YOUR IMPLEMENTATION STOPS HERE
};

// implement a free function initializing a Widget using
// a non-reference 'std::vector' as argument (pass-by-value)
Widget init_byvalue(Vector vec){
    // YOUR IMPLEMENTATION GOES HERE
    return Widget{}; // adopt this line as needed;
    // YOUR IMPLEMENTATION STOPS HERE
};

// 'main' measures the runtime for
// initialization using a rvalue reference to 'std::vector'(test1) and
// initialization using a const lvalue reference to 'std::vector'(test2)
int main() {

  size_t size = 1'000'000; // vector length 1M == ~7MB
  size_t n = 10;           // iterations for averaging run time

  auto test_rref = [size]() {
    Vector vec(size, std::rand());
    Widget w = init(std::move(vec));
    if (vec.data() != nullptr) {
      std::cout << "vector was not moved from rvalue, this can be improved"
                << std::endl;
    }
  };
  auto test_lref = [size]() {
    Vector vec(size, std::rand());
    Widget w = init(vec);
    if (vec.data() == nullptr) {
      std::cout << "vector was moved from lvalue, should not happen"
                << std::endl;
    }
    if (vec.size() != w.vec.size() || vec[1] != w.vec[1]) {
      std::cout
          << "vectors of not equal size, or different value, should not happen"
          << std::endl;
    }
  };
  auto test_byvalue = [size]() {
    Vector vec(size, std::rand());
    Widget w = init_byvalue(vec);
    if (vec.data() == nullptr) {
      std::cout << "vector was moved from lvalue, should not happen"
                << std::endl;
    }
    if (vec.size() != w.vec.size() || vec[1] != w.vec[1]) {
      std::cout
          << "vectors of not equal size, or different value, should not happen"
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
  std::cout << runtest(test_rref) << "s" << std::endl;
  std::cout << runtest(test_lref) << "s" << std::endl;
  std::cout << runtest(test_byvalue) << "s" << std::endl;
}