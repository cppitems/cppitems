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
  // you task it to implement the three SMF below according to the needs of this
  // resource owning class
  Vector(const Vector &vec) = default; // (1) defaulted copy-ctor (defective),
  Vector(Vector &&vec) = default;      // (2) defaulted move-ctor (defective)
  ~Vector() = default;                 // (3) defaulted dtor (defective)

  value_type *data() { return dataPointer; }
  size_type size() { return dataSize; }
  value_type &operator[](size_type pos) { return dataPointer[pos]; }
};