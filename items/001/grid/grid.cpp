#include <cstdlib> // std::free, std::calloc
#include "grid.h"
void grid_init(GridType *grid) {
  grid->nx = 3;
  grid->ny = 4;  
  grid->data = (double *)std::calloc(grid->nx * grid->ny, sizeof(double));
}
void grid_free(GridType *grid) {
  std::free(grid->data);
  grid->nx = 0;
  grid->ny = 0;
}
double *grid_at(GridType *grid, int x, int y) {
  return &grid->data[x + grid->nx * y];
}