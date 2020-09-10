/* file: grid.c */ /* compile: clang -std=c11 -c grid.c */
#include "grid.h"
void grid_init(struct Grid *grid, size_t nx, size_t ny) {
  grid->data = (double *)calloc(nx * ny, sizeof(double));
  grid->nx = nx;
  grid->ny = ny;  
}
void grid_free(struct Grid *grid) {
  free(grid->data);
  grid->nx = 0;
  grid->ny = 0;
}
double *grid_at(struct Grid *grid, size_t x, size_t y) {
  return &grid->data[x + grid->nx * y];
}
