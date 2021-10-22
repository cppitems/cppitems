1 // item status
# Exercise EX0

The exercise is available as a git repository including here:
https://github.com/cppitems/ex0

# Possible solutions

## A: Full implementation with some redundancy
```pmans
// Option A: "full own impl"
#include <cstddef>
struct Vector {
  using value_type = int;
  Vector::value_type *_data = nullptr;
  size_t _size;
  Vector(size_t size, value_type init)
      : _data(new value_type[size]), _size(size) {
    for (size_t i = 0; i != _size; ++i) {
      _data[i] = init;
    }
  }
  ~Vector() { delete[] _data; }
  Vector(const Vector &other)
      : _data(new value_type[other._size]), _size(other._size) {
    for (size_t i = 0; i != _size; ++i) {
      _data[i] = other._data[i];
    }
  }
  Vector &operator=(const Vector &other) {
    delete[] _data;
    _data = new value_type[other._size];
    _size = other._size;
    return *this;
  }
  size_t size() const { return _size; }
  value_type *data() { return _data; }
  value_type &at(size_t idx) { return _data[idx]; }
  value_type sum() {
    value_type sum = {0};
    for (size_t i = 0; i != _size; ++i) {
      sum += _data[i];
    }
    return sum;
  }
};
```
## C: Own implementation using some stdlib functionality to reduce redundancy
```pmans
#include <cstddef> // size_t
#include <memory> // unique_ptr
#include <numeric> // accumulate
#include <utility> // swap
struct Vector {
  using value_type = int;
  std::unique_ptr<value_type[]> _data;
  size_t _size;
  Vector(size_t size, value_type init)
      : _data(std::make_unique<value_type[]>(size)), _size(size) {
    std::fill(_data.get(), _data.get() + _size, init);
  }
  Vector(const Vector &other)
      : _data(std::make_unique<value_type[]>(other._size)), _size(other._size) {
    std::copy(other._data.get(),other._data.get() + other._size, _data.get() );
  }
  Vector &operator=(Vector other) {
    std::swap(_size, other._size);
    std::swap(_data, other._data);
    return *this;
  }
  size_t size() const { return _size; }
  value_type *data() { return _data.get(); }
  value_type &at(size_t idx) { return _data[idx]; }
  value_type sum() {
    return std::accumulate(_data.get(), _data.get() + _size, value_type{0});
  }
};
```
## B: Inherit from std::vector and only implement "extras"
```pmans
#include <numeric>
#include <vector>
using Base = std::vector<int>;
struct Vector : Base {
  // using Base::Base; // all ctors
  Vector(size_t size, Base::value_type init) : Base(size, init) {}
  Base::value_type sum() {
    return std::accumulate(Base::begin(), Base::end(), Base::value_type{0});
  }
};
```