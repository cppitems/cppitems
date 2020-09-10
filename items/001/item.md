5 // item status
# 2D Grid Class in C
## Example of a library providing an interface to work with 2D regular grid data

This item introduces a simplistic interface to work with a `Grid` *structure* which hold data of a 2D regular grid.
A `grid_init` function allocates the grid with the desired dimension and performs the initialization.
To access the grid at a 2D coordinate, `grid_at` returns a *pointer* to the respective position in the grid. 
Finally, to `grid_free` is used to *deallocate* the memory and reset the structure. So the *header* file looks like this: 

```pmans 
/* file: grid.h */ ///
#pragma once ///
#include <stdlib.h> /// ...
struct /*b4*/ Grid {
  size_t nx;
  size_t ny;
  double *data;
};
void /*b9*/ grid_init(struct Grid *grid, size_t nx, size_t ny);
void /*b9*/ grid_free(struct Grid *grid);
double */*b7*/ grid_at(struct Grid *grid, size_t x, size_t y);
```

 A usage of this interface can look like

 ```pmans
 /* file: main.c */ /* compile: clang -std=c11 grid.o main.c -o main */ ///
#include "grid.h" ///
#include <stdlib.h> ///
int main() { /// ...
  struct /*b4*/ Grid grid = {0};
  /*b9*/ grid_init(&grid, 3, 4);
  for(size_t x = 0; x != grid.nx; ++x) {
    for (size_t y = 0; y != grid.ny; ++y) {
      */*b7*/ grid_at(&grid, x, y) = (double)x + y;
    }
  }
  /*b9*/ grid_free(&grid);
  return 0; ///
} /// ...
 ```
where a 2D sweet over the grid is performed using a nested for-loop and the `grid_at` function is used to write to the grid. This library only uses language features from the C language. It serves as a starting point to introduce C++ language features and discuss how these features can be used to (hopefully) improve several aspects of this library and its utilization.
