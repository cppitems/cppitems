#include <iostream>
#include <iterator>
#include <list>
#include <vector>

struct Widget {
  std::vector<double> data;
  std::vector<bool> active;
  Widget(std::size_t size) : data(size, 0.0), active(size, false) {}
  void set_value(std::size_t idx, double value) { data[idx] = value; }
  void set_status(std::size_t idx, bool status) { active[idx] = status; }
  struct IteratorActive {
    double &operator*();
    IteratorActive &operator++();
    bool operator!=(const IteratorActive &other);
    IteratorActive begin_active();
    IteratorActive end_active();
  };
};

int main() {
  {
    Widget w(10);
    w.set_status(0, true);
    w.set_value(0, 0);
    w.set_status(3, true);
    w.set_value(3, 3);
    w.set_status(9, true);
    w.set_value(9, 9);

    // iterate over 'active' values in Widget
    for (std::size_t i = 0; i < w.data.size(); ++i) {
      if (w.active[i]) {
        // read/write using 'w.data[i]'
        std::cout << w.data[i] << std::endl;
      }
    }
    // iterate over 'active' values in Widget
    // for (auto iter = w.begin_active(); iter != w.end_active(); ++iter) {
    //   [[maybe_unused]] double &value = *iter;
    // }
  }
}
