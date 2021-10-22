// Option C: "own impl" + std
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