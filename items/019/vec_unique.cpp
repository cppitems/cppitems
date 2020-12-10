#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <utility>
#include <vector>

struct Base {
  virtual int calculate() { return 5; }
  virtual ~Base() = default;
};
struct Widget1 : public Base {
  int calculate() override { return 1; }
};
struct Widget2 : public Base {
  int calculate() override { return 2; }
};

int main() {
  std::vector<std::unique_ptr<Base>> vec;
  vec.emplace_back(new Widget1{});
  vec.emplace_back(new Widget2{});
  vec.emplace_back(new Base{});

  for (auto &&item : vec) {
    std::cout << item->calculate() << std::endl;
  }

  //   std::sort(vec.begin(), vec.end()); // (1a) sorts by address
  //   auto vec2 =vec; // does not work, using unique_ptr
  //   std::vector<std::shared_ptr<Base>> vec_shared;
  //   vec_shared.emplace_back(new WidgetA{{3}, 3});
  //   vec_shared.emplace_back(new WidgetA{{2}, 2});
  //   vec_shared.emplace_back(new WidgetA{{1}, 1});
  //   auto vec2 = vec_shared; // does work, is a shallow copy
}