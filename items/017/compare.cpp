#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int main() {
  struct Widget {
    int m;
  };
  std::vector<Widget> vec{};
  vec.push_back(Widget{10});
  vec.push_back(Widget{5});
  vec.push_back(Widget{4});

  auto lambda = [](const Widget &a, const Widget &b) { return a.m < b.m; };
  //   int threshold = 2;
  //   auto lambda = [threshold](const Widget &a, const Widget &b) {
  //     return std::abs(a.m - b.m) > threshold ? a.m < b.m : false;
  //   };

  std::sort(vec.begin(), vec.end(), lambda);
  for (auto &&item : vec) {
    std::cout << item.m << " ";
  }
}