#include <initializer_list>

template <typename T = double> 
struct Pair {
  T first;
  T second;
  Pair operator+(const Pair &other) {
    return Pair{first + other.first, second + other.second};
  }
};

// template<typename T>
// Pair(std::initializer_list<T>) -> Pair<T>;  // works 
// template<typename T>
// Pair(T, T) -> Pair<T>;  // also works, but this is not a constructor


int main(){
    auto pair1 = Pair{1,2}; // not working without deduction guide
}