#include "grid.h"
int main() {
  GridType grid;
  grid_init(&grid);
  bool b;
  // *grid_at(&grid, 0, 0) = b; // (1) compiler warning
  auto useb = [&grid, &b]() {
    *grid_at(&grid, 0, 0) = b; // (2) no compiler warning
  };
  useb();
  if (*grid_at(&grid, 0, 0)) {
  } // (3) first 'read' of 'b'
  grid_free(&grid);
  return 0;
}