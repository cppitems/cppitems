#pragma once
#include <memory>
#include <random>

namespace library {

  // interface (base) for the user provided implementation
  struct GeneratorBase {
    virtual double operator()() = 0; // Task2: (1) pure virtual
    // virtual double operator()() { return 0; }; // Task2: (2) virtual
    virtual ~GeneratorBase() {}
  };

  // non templated interface to library
  class RandomPi {
    // shared pointer to generator base
    std::shared_ptr<GeneratorBase> genPtr;

  public:
    RandomPi(std::shared_ptr<GeneratorBase>);

    // function to approximate Pi
    double calculate(unsigned long long);
  };

}