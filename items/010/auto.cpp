#include <initializer_list>
struct Widget {
  int m;
};

// identical type deduction mechanism for 'auto' and function template argument

template <typename AUTO> void func(AUTO &&arg){};

int main() {
  Widget expr{1};
  func(expr); // the type deduced of arg (AUTO&&) in func is identical to ...
  auto &&arg = (expr); // ... the type of arg deduced here using auto&&
}

// // example for only difference: std::initializer_list
// template <typename AUTO>
// void func(std::initializer_list<AUTO> arg){};
// int main() {
//   auto arg = {1, 2, 3}; // works
//   func({1, 2, 3});      // does not work
// }