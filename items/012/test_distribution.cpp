#include "Distribution.hpp"

#include "SpaceVector.hpp"

#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include <vector>

// Requirements for Type Distribution:
// - Default constructor for explicit instantiation
// - Template constructor taking any STL data structure as sole parameter
// - Class is a template of the same type of the STL data structures value_type
// - Class has a typedef called value_type exposing its template type
// - Members mean and stddev hold the mean and standard deviation
//   of the data stored in the passed datastructure repectively

template <class C>
Distribution<double> populate_with_distribution(C &container) {
  using T = typename C::value_type;
  const std::size_t size = 1'000'000;

  // setup varying normal distribution
  std::mt19937 gen(1); // PRNG
  std::uniform_real_distribution<> uniDist(0.0, 1.0);

  // Explicitly initialise Distribution
  Distribution<double> initial(uniDist(gen) * 100.0, uniDist(gen) * 10.0);

  // populate container by drawing from normal distribution
  std::normal_distribution<> normalDist{initial.mean, initial.stddev};
  for (unsigned i = 0; i < size; ++i) {
    T value(normalDist(gen));
    container.insert(container.end(), std::move(value));
  }

  // return Distribution
  return initial;
}

template <class D1, class D2> void print_difference(D1 &initial, D2 &final) {
  std::cout << "    mean = " << std::setw(5) << std::fixed
            << (final.mean - initial.mean)
            << "; stddev = " << (final.stddev - initial.stddev) << std::endl;
}

template <class T> void test_containers() {
  {
    std::cout << "  Vector: " << std::endl;
    using CTNR = std::vector<T>;
    CTNR ctnr;
    Distribution<double> init_distr = populate_with_distribution(ctnr);
    Distribution final_distr(ctnr);
    print_difference(init_distr, final_distr);
  }
  {
    std::cout << "  List: " << std::endl;
    using CTNR = std::list<T>;
    CTNR ctnr;
    Distribution<double> init_distr = populate_with_distribution(ctnr);
    Distribution final_distr(ctnr);
    print_difference(init_distr, final_distr);
  }
  {
    std::cout << "  Deque: " << std::endl;
    using CTNR = std::deque<T>;
    CTNR ctnr;
    Distribution<double> init_distr = populate_with_distribution(ctnr);
    Distribution final_distr(ctnr);
    print_difference(init_distr, final_distr);
  }
}

int main() {
  std::cout << "Differences between original distribution and randomly "
               "generated distribution:"
            << std::endl;
  {
    std::cout << "double:" << std::endl;
    test_containers<double>();
  }
  {
    std::cout << "long:" << std::endl;
    test_containers<long>();
  }
  {
    std::cout << "float:" << std::endl;
    test_containers<float>();
  }
  {
    std::cout << "SpaceVector<double>:" << std::endl;
    test_containers<SpaceVector<double>>();
  }

  return 0;
}
