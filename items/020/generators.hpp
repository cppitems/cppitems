#pragma once
#include "librandompi/RandomPi_headeronly.hpp" // namespace headeronly: templated library interface
#include "librandompi/RandomPi_library.hpp" // namespace library: template-free library interface
#include <random>


namespace library {
  // Task1: implement derived class (req. enforced by abstract base class)
  template <class EngineType> class Generator : public GeneratorBase {

  public:
    Generator(typename EngineType::result_type seed) {}
    double operator()() override { return 0; }
  };
}

namespace headeronly {
  // Task1: implement template class (req. enforeced by static_asserts)
  template <class EngineType> class Generator {

  public:
    Generator(typename EngineType::result_type seed) {}
    double operator()() { return 0; }
  };
}