#include <string>
#include <utility>
template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename T = double> struct Pair {
  T first;
  T second;
  Pair(const T &first, const T &second) : first(first), second(second) {}
  auto operator+(const Pair &other) {
    return Pair{first + other.first, second + other.second};
  }
};

template <> struct Pair<char> {
  char first;
  char second;
  Pair(const char &first, const char &second) : first(first), second(second) {}
  auto operator+(const Pair &other) {
    return Pair<std::string>{
        std::string(1, first) + std::string(1, other.first),
        std::string(1, second) + std::string(1, other.second)};
  }
};

int main() {
  { // usage equal types
    Pair p1('a', 'a');
    Pair p2('b', 'a');
    auto sum = p1 + p2; // DebugType<decltype(sum)> error;
  }
}