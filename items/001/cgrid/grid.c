/* file: grid.c */ /* compile: clang -std=c11 -c grid.c */
#include <stdlib.h> // for calloc
#include "grid.h" 
void /*b9*/ grid_init(/*f9*/ GridType *grid, int nx, int ny) {
  grid->data = (double *)calloc(nx * ny, sizeof(double));
  grid->nx = nx;
  grid->ny = ny;  
}
void /*b9*/ grid_free(/*f9*/ GridType *grid) {
  free(grid->data);
  grid->nx = 0;
  grid->ny = 0;
}
double */*b7*/ grid_at(/*f9*/ GridType *grid, int x, int y) {
  return &grid->data[x + grid->nx * y];
}
