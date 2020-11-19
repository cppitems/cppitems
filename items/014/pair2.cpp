#include <string>
#include <utility>
template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename FIRST, typename SECOND> struct Pair {
  FIRST first;
  SECOND second;
  Pair(const FIRST &first, const SECOND &second)
      : first(first), second(second) {}
  auto operator+(const Pair &other) {
    return Pair{first + other.first, second + other.second};
  }
//   template <typename OTHERPAIR> auto operator+(const OTHERPAIR &other) {
//     return Pair{first + other.first, second + other.second};
//   }
};

// template <typename PAIR1, typename PAIR2>
// auto operator+(const PAIR1 &a, const PAIR2 &b) {
//     return Pair{a.first + b.first, a.second + b.second};
// }

int main() {
  { // usage equal types
    Pair p1(1.0, 2.0);
    Pair p2(2.0, 1.0);
    auto sum = p1 + p2;
  }
//   { // usage mixed types
//     Pair p1(1, 2.0);
//     Pair p2(2.0, 1);
//     auto sum = p1 + p2;
//   }
}