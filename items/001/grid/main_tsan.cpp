#include "grid.h"
#include <mutex>
#include <omp.h>
#include <thread>
int main() {
  GridType grid;
  grid_init(&grid);

  //  std::mutex write_mutex;
  //   auto write_grid = [&grid, &write_mutex]() {
  //     const std::lock_guard<std::mutex> lock(write_mutex);
  //     *grid_at(&grid, 0, 0) = 5;
  //   };

   auto write_grid = [&grid]() { *grid_at(&grid, 0, 0) = 5; };

  std::thread one(write_grid);
  std::thread two(write_grid);
  one.join();
  two.join();

  grid_free(&grid);

  return 0;
}
