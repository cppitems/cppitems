#include <initializer_list>
#include <tuple>

template <typename T> struct DebugType { using T::notexisting;};
// usage: DebugType<decltype(...)> error;

template <typename AUTO> void func(const AUTO &expr){}; // (1)
int main() {                                            // (2)
  auto func_int = func<int>;                            // (3)
  auto a = 2;                                           // (4)
  const auto b = 2.0;                                   // (5)
  double c = a + 2;                                     // (6)
  auto d = {2};                                         // (7)
  decltype(d) e;                                        // (8)
  struct Widget {
    int i;
  };                                                         // (9)
  auto lambda = [=](Widget w) { return w.i + a + b; };       // (10)
  auto res = lambda(Widget{2});                              // (11)
  auto lambda2 = [&]() { return std::make_tuple(a, b, c); }; // (12)
  auto [one, two, three] = lambda2();                        // (13)
  auto str = "C++";                                          // (14)
  decltype("C++") f{};                                       // (15) 
  return 0;
}
