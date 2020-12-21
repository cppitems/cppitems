#include "RandomPi_library.hpp"

namespace library {

  RandomPi::RandomPi(std::shared_ptr<GeneratorBase> passedGen)
      : genPtr(passedGen) {}

  double RandomPi::calculate(unsigned long long accuracy) {
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

}