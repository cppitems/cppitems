/* file: main.c */ /* compile: clang -std=c11 grid.o main.c -o main */
#include "grid.h"
int main() {
  /*f8*/ GridType /*f4*/ grid = {0};
  /*b9*/ grid_init(&/*f4*/ grid, 3, 4);
  for (int x = 0; x != grid.nx; ++x) {
    for (int y = 0; y != grid.ny; ++y) {
      */*b7*/ grid_at(&/*f4*/ grid, x, y) = (double)x + y;
    }
  }
  /*b9*/ grid_free(&/*f4*/ grid);
  return 0;
}