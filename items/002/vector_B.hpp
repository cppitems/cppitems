// Option B: inheritance from std::vector + "own sum()"
#include <numeric>
#include <vector>
using Base = std::vector<int>;
struct Vector : Base {
  // using Base::Base; // all ctors
  Vector(size_t size, Base::value_type init) : Base(size, init) {}
  Base::value_type sum() {
    return std::accumulate(Base::begin(), Base::end(), Base::value_type{0});
  }
};
