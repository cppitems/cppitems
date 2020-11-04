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

  int lower = 2;
  int upper = 5;
  auto lambda = [&t1 = lower, &t2 = upper](const Widget &a) {
    return a.m > t1 && a.m < t2;
  };

  {
    auto count = std::count_if(vec.begin(), vec.end(), lambda);
    std::cout << count << std::endl;
  }
  ++upper;
  {
    auto count = std::count_if(vec.begin(), vec.end(), lambda);
    std::cout << count << std::endl;
  }
}