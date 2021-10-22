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