#include <iostream>

struct Widget {
  double m;
};

template <typename T, int N> struct WidgetArray {
  T data[N];
  T *begin() { return data; }
  T *end() { return data + N; }
};

template <typename T, int N> T *begin(WidgetArray<T, N> &array) {
  return array.data;
}
template <typename T, int N> T *end(WidgetArray<T, N> &array) {
  return array.data + N;
}
int main() {
  using Array = WidgetArray<Widget, 10>;
  Array array;
  int count = 0;
  for (auto &&item : array) {
    // item = ++count;
  }
  for (auto &&item : array) {
    std::cout << item.m << std::endl;
  }
}