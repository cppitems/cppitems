#include "generators.hpp" // implemenation of user provided types for random number generation
#include "librandompi/RandomPi_headeronly.hpp" // namespace headeronly: templated library interface
#include "librandompi/RandomPi_library.hpp" // namespace library: template-free library interface
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>

int main() {

  auto benchmark = [](auto &&name, auto &&pi, double accuracy) {
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<double>;
    std::cout << name << std::endl;
    std::cout << "    accuracy: " << std::setprecision(0) << std::scientific
              << accuracy;
    auto start = Clock::now();
    std::cout << "; pi_approx: " << std::fixed << pi.calculate(accuracy);
    auto stop = Clock::now();
    std::cout << "; time: " << std::setprecision(5) << std::scientific
              << Duration(stop - start).count() << std::endl;
  };

  int seed = 1;
  size_t accuracy = 5e7;
  std::cout << "std::minstd_rand" << std::endl;
  {
    auto gen = std::make_shared<headeronly::Generator<std::minstd_rand>>(seed);
    headeronly::RandomPi pi(gen);
    benchmark("  headeronly", pi, accuracy);
  }
  {
    auto gen = std::make_shared<library::Generator<std::minstd_rand>>(seed);
    library::RandomPi pi(gen);
    benchmark("  library", pi, accuracy);
  }
  std::cout << "std::mt19937" << std::endl;
  {
    auto gen = std::make_shared<headeronly::Generator<std::mt19937>>(seed);
    headeronly::RandomPi pi(gen);
    benchmark("  headeronly", pi, accuracy);
  }
  {
    auto gen = std::make_shared<library::Generator<std::mt19937>>(seed);
    library::RandomPi pi(gen);
    benchmark("  library", pi, accuracy);
  }
}