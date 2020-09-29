#pragma once
#include <cstdlib> // std::free, std::calloc
struct Grid {
  int nx;
  int ny;
  double *data;
  void init() {
    this->nx = 3;
    this->ny = 4;
    this->data = (double *)std::calloc(this->nx * this->ny, sizeof(double));
  }
  void free() {
    std::free(this->data);
    this->nx = 0;
    this->ny = 0;
  }
  double *at(int x, int y) { return &this->data[x + this->nx * y]; }
  void resize(size_t x, size_t y) {
    double *data = (double *)std::calloc(this->nx * this->ny, sizeof(double));
    std::free(this->data);
    this->data = data;
  }
};
