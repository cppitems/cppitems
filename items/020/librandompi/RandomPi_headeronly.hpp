#pragma once
#include <memory>
#include <type_traits>

namespace headeronly {
  // templated interface to libary (type template parameter is user provided)
  template <class Generator> class RandomPi {
    // shared pointer to generator base
    std::shared_ptr<Generator> genPtr;

  public:
    RandomPi(std::shared_ptr<Generator> passedGen) : genPtr(passedGen) {
      static_assert(std::is_invocable<decltype((*genPtr))>::value,
                    "generator.operator() is not avaialbled");
      static_assert(std::is_same<decltype((*genPtr)()), double>::value,
                    "return value of generator.operator() is not double");
    }
    // function to approximate Pi
    double calculate(unsigned long long accuracy) {
      unsigned long long inside = 0;
      auto &gen = *genPtr;

      for (unsigned long long i = 0; i < accuracy; ++i) {
        double x = gen();
        double y = gen();
        if (x * x + y * y < 1) {
          ++inside;
        }
      }
      return 4.0 * double(inside) / double(accuracy);
    }
  };

}