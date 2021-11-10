#include <string>
#include <utility>
template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename FIRST, typename SECOND> struct Pair {
  FIRST first;
  SECOND second;
  Pair(const FIRST &first, const SECOND &second)
      : first{first}, second{second} {}
  //   auto operator+(const Pair &other) {
  //     return Pair{first + other.first, second + other.second};
  //   }
  //   template <typename OTHERPAIR>
  //   auto operator+(const OTHERPAIR &other) {
  //     return Pair{first + other.first, second + other.second};
  //   }
};

// global operator overload for operator+(A,B)
template <typename T, typename U, typename T2, typename U2>
auto operator+(const Pair<T, U> &a, const Pair<T2, U2> &b) {
  return Pair{a.first + b.first, a.second + b.second};
}

int main() {

  //   {  usaging different but compatible types
  //     Pair p1(1, 2.0); // Pair<int,double>
  //     Pair p2(2.0, 1); // Pair<double,int>
  //     [[maybe_unused]] Pair<double, double> sum = p1 + p2;
  //     // type of sum?
  //   }
  // { using different incompatible types
  //   Pair p1(1, 2.0);
  //   Pair p2(std::string("d"), 5);
  //   auto sum = p1 + p2;
  // type of sum?
  // }
}