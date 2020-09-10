/* file: main.c */ /* compile: clang -std=c11 grid.o main.c -o main */
#include "grid.h"
#include <stdlib.h>
int main() {
  struct Grid grid = {0};
  grid_init(&grid, 3, 4);
  for (size_t x = 0; x != grid.nx; ++x) {
    for (size_t y = 0; y != grid.ny; ++y) {
      *grid_at(&grid, x, y) = (double)x + y;
    }
  }
  grid_free(&grid);
  return 0;
}