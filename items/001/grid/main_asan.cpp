#include "grid.h"
int main() {
  GridType grid;
  grid_init(&grid);
  for (int x = 0; x != grid.nx; ++x) {
    for (int y = 0; y != grid.ny + 1; ++y) {
      *grid_at(&grid, x, y) = x + y;
    }
  }
//   grid_free(&grid); // runtime memory leak
  return 0;
}