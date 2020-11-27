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
  void push_back(bool status, double value) {
    data.push_back(value);
    active.push_back(status);
  }
  struct IteratorActive {
    Widget &ref;
    std::size_t pos;
    std::size_t end;
    double &operator*() { return ref.data[pos]; }
    IteratorActive &operator++() {
      while (++pos != end) {
        if (ref.active[pos])
          break;
      }
      return *this;
    }
    bool operator!=(const IteratorActive &other) {
      return this->pos != other.pos;
    }
  };
  IteratorActive begin_active() {
    IteratorActive iter = {*this, 0, data.size()};
    return iter.ref.active[iter.pos] ? iter : ++iter;
  }
  IteratorActive end_active() {
    return IteratorActive{*this, data.size(), data.size()};
  }
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
    for (auto iter = w.begin_active() ; iter !=  w.end_active(); ++iter) {
      [[maybe_unused]] double &value = *iter;
    }
  }
}
