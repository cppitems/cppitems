#include "grid.h"
#include <mutex>
#include <omp.h>
#include <thread>
int main() {
  GridType grid;
  grid_init(&grid);

  //   // threads
  std::mutex write_mutex;
  auto write_grid = [&grid, &write_mutex]() {
    const std::lock_guard<std::mutex> lock(write_mutex);
    *grid_at(&grid, 0, 0) = 5;
  };
  std::thread one(write_grid);
  std::thread two(write_grid);
  one.join();
  two.join();

  // omp for
  omp_set_num_threads(2);
#pragma omp parallel
  {
#pragma omp critical
    { *grid_at(&grid, 0, 0) = 5; }
  }

// omp task
#pragma omp parallel
  {
#pragma omp single
    {
#pragma omp task
#pragma omp critical
      { *grid_at(&grid, 0, 0) = 5; }
#pragma omp task
#pragma omp critical
      { *grid_at(&grid, 0, 0) = 5; }
    }
  }
  grid_free(&grid);

  return 0;
}