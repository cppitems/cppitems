#include "grid.h"
int main() {
  Grid grid;
  grid_init(&grid);
  *grid_at(&grid, 0, 0) = 5;
  grid_resize(&grid, 10, 10);
  *grid_at(&grid, 9, 9) = 5;
  grid_free(&grid);
  return 0;
}