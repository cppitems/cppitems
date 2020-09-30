#pragma once
#include <cstdlib> // std::free, std::calloc
struct Grid {
  int nx;
  int ny;
  double *data;
};

void grid_init(Grid *that) {
  that->nx = 3;
  that->ny = 4;
  that->data = (double *)std::calloc(that->nx * that->ny, sizeof(double));
};
void grid_free(Grid *that) {
  std::free(that->data);
  that->nx = 0;
  that->ny = 0;
}
double *grid_at(Grid *that, int x, int y) {
  return &that->data[x + that->nx * y];
}
void grid_resize(struct Grid *that, size_t x, size_t y) {
  double *data = (double *)std::calloc(that->nx * that->ny, sizeof(double));
  std::free(that->data);
  that->data = data;
}
