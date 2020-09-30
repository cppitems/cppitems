#pragma once
#include <cstddef>
#include <algorithm>
#include <cstdlib> // std::free, std::calloc
struct Grid {
  int nx;
  int ny;
  double *data;
  Grid() : nx(3), ny(4), data(new double[nx * ny]) {
    std::fill_n(data, nx * ny, 0);
  }
  ~Grid() { delete[] data; }
  double *at(int x, int y) { return &this->data[x + this->nx * y]; }
};
