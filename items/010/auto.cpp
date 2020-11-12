#include <initializer_list>
struct Widget {
  int m;
};

template <typename AUTO> void func(AUTO arg){};

int main() {
  Widget expr{1};    //
  func(expr);        // type of arg
  auto arg = (expr); // type of arg
}

// template <typename AUTO> void func(std::initializer_list<AUTO> arg){};
// int main() {
//   auto arg = {1, 2, 3}; // works
//   func({1, 2, 3});      // does not work
// }