#include <cstddef>

template <class T> class Vector {
public:
  using value_type = T;
  using size_type = std::size_t;

private:
  size_type dataSize = 0;
  value_type *dataPointer = nullptr;

public:
  Vector() {}
  Vector(size_type count, const T &value)
      : dataSize(count), dataPointer(new T[count]) {
    for (std::size_t i = 0; i < count; ++i) {
      dataPointer[i] = value;
    }
  }
  Vector(const Vector &vec) = default;
  Vector(Vector &&vec) = default;
  ~Vector() = default;

  value_type *data() { return dataPointer; }
  size_type size() { return dataSize; }
  value_type &operator[](size_type pos) { return dataPointer[pos]; }
};