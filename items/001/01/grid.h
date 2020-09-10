/* file: grid.h */
#pragma once
#include <stdlib.h>
struct Grid {
  size_t nx;
  size_t ny;
  double *data;
};
void grid_init(struct Grid *grid, size_t nx, size_t ny);
void grid_free(struct Grid *grid);
double *grid_at(struct Grid *grid, size_t x, size_t y);
