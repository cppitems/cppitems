#include "grid.h"
#include <thread>
#include <mutex>
int main(int argc, char** argv) {
//   int* a = new int[10];
//   a[5] = 0;
//   volatile int b = a[argc];
//   if (b)
//     printf("xx\n");

  double b;
  GridType grid;
  grid_init(&grid);
  std::mutex sweep_mutex;  
  auto sweep = [&grid, &b, &sweep_mutex]() {
  for (int x = 0; x != grid.nx; ++x) {
    for (int y = 0; y != grid.ny+1; ++y) {
      const std::lock_guard<std::mutex> lock(sweep_mutex);
    //   if (b)
    // double gg = b;
      *grid_at(&grid, x, y) = b;
      
      if (*grid_at(&grid, x, y))
        ++b;
    }
  }
  };

//   sweep();  
  std::thread one(sweep);
  std::thread two(sweep);
  one.join();    
  two.join();  
  grid_free(&grid);

  return 0;
}