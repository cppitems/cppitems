#include "grid.h"
int main() {
  Grid grid;
  grid.init();
  *grid.at(0, 0) = 5;
  grid.resize(10, 10);
  *grid.at(9, 9) = 5;
  grid.free();
  return 0;
}