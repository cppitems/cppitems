#include <string>
#include <type_traits>
#include <utility>
template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

// using T = double;
// struct Pair {
//   T first;
//   T second;
//   Pair operator+(const Pair &other) {
//     return Pair{first + other.first, second + other.second};
//   }
// };

template <typename T = double> struct Pair {
  T first;
  T second;

  template <typename U >
  Pair(const T &first, const U &second) : first(first), second(second) {
    //   static_assert(std::is_floating_point<U>::value);
  }
  auto operator+(const Pair &other) {
    return Pair{first + other.first, second + other.second};
  }
};

int main() {
//   { // usage template double
//     Pair p1(1,1.0f);
//     // DebugType<decltype(p1)> error;
//   }    
    // { // usage non-template
    //   Pair p1{1.0, 2.0};
    //   Pair p2{2.0, 1.0};
    //   auto sum = p1 + p2;
    // }
//   { // usage template double
//     Pair<double> p1{1, 2};
//     Pair<double> p2{2, 1};
//     auto sum = p1 + p2;
//   }
//   { // usage template int
//     Pair<int> p1{1, 2};
//     Pair<int> p2{2, 1};
//     auto sum = p1 + p2;
//   }
//   { // usage template string
//     Pair<std::string> p1{"1", "2"};
//     Pair<std::string> p2{"2", "1"};
//     // DebugType<decltype(p1)> error;
//     auto sum = p1 + p2;
//   }
//   { // usage template string
//     Pair p1{std::string("1"), std::string("2")};
//     Pair p2{std::string("2"), std::string("1")};
//     auto sum = p1 + p2;
//   }
//   { // default arg
//     Pair<> p1(1, 2);
//     Pair<> p2(2, 3);
//     auto sum = p1 + p2;
//   }
}