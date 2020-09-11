#include "grid.h"
int main() {
  GridType grid;
  grid_init(&grid);
  for (int x = 0; x != grid.nx; ++x) {
    for (int y = 0; y != grid.ny; ++y) {
      *grid_at(&grid, x, y) = (double)x + y;
    }
  }
  grid_free(&grid);
  return 0;
}