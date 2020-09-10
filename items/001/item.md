5 // item status
# 2D Grid library (C)
## Simple interface to work with 2D regular grid data

This item introduces a simplistic interface to work with a `Grid` *structure* which hold data representing a 2D regular grid.
A `grid_init` function allocates the grid with the desired dimension and performs the initialization.
To access the grid at a coordinate, `grid_at` returns a *pointer* to the respective item in the grid. 
Finally, `grid_free` is used to *deallocate* the memory and reset the structure. So the *header* file looks like this: 

```pmans 
/* file: grid.h */
#pragma once 
#include <stdlib.h> 
struct /*f4*/ Grid {
  size_t nx;
  size_t ny;
  double *data;
};
typedef struct Grid /*f8*/ GridType;
void /*b9*/ grid_init(/*f9*/ GridType *grid, size_t nx, size_t ny);
void /*b9*/ grid_free(/*f9*/ GridType *grid);
double */*b7*/ grid_at(/*f9*/ GridType *grid, size_t x, size_t y);
```

and the associated *source* file which *defines* the interface functions looks like
```pmans
/* file: grid.c */ /* compile: clang -std=c11 -c grid.c */
#include <stdlib.h> // for calloc
#include "grid.h"
void /*b9*/ grid_init(struct Grid *grid, size_t nx, size_t ny) {
  grid->data = (double *)calloc(nx * ny, sizeof(double));
  grid->nx = nx;
  grid->ny = ny;  
}
void /*b9*/ grid_free(struct Grid *grid) {
  free(grid->data);
  grid->nx = 0;
  grid->ny = 0;
}
double */*b7*/ grid_at(struct Grid *grid, size_t x, size_t y) {
  return &grid->data[x + grid->nx * y];
}

```

 A usage of this interface can then look like

 ```pmans
/* file: main.c */ /* compile: clang -std=c11 grid.o main.c -o main */
#include "grid.h"
int main() {
  /*f8*/ GridType /*f4*/ grid = {0};
  /*b9*/ grid_init(&/*f4*/ grid, 3, 4);
  for (size_t x = 0; x != grid.nx; ++x) {
    for (size_t y = 0; y != grid.ny; ++y) {
      */*b7*/ grid_at(&/*f4*/ grid, x, y) = (double)x + y;
    }
  }
  /*b9*/ grid_free(&/*f4*/ grid);
  return 0;
}
 ```
where a sweet over the grid is performed using a nested for-loop.
The `grid_at` function is used to write to the grid. 
This library only uses language features from the C language. 
It serves as a starting point to introduce C++ language features and discuss how these features can be used to (hopefully) improve several aspects of this library and its utilization.

## File organization
The library source code is organized using *header* and *source* files.
(It could also have been placed in a single file 
This separation has some advantages:
- all *declarations* contributing to the interface are available from the header file
- the library implementation can be compiled in a separate *compilation unit* which procudes an *object* file (or a *library*)
- allows recompilation of parts of an application 
- the implementation can be distributed in compiled form
- it can be distributed as a *shared* library
- implementation changes are separated from interface changes

Disadvantages of the separation are:
- a portion of the sourcecode is redundant (the function declarations)
- an optimizing compiler optimizes each compilation unit separately (optimization accross compilation units is not common)
- compatibility consideration (when distributen in compiled form)
- extra steps build process (linking)
- need for recompilation if global project configuration changes (compiler flags, multithreading) 

## Compilation Process
To output the full set of flags when compiling the library  can be revealed with
```pmans
/* compile: clang -### grid.c -c */ // show full invocation arguments
```
which will list the default settings assuming the host configuration as compilation target.

Further, a
```pmans
/* compile: clang -ccc-print-phases grid.c -c */ // show clang compilation stages
```
will print something like
```
         +- 0: input, "grid.c", c 
      +- 1: preprocessor, {0}, cpp-output
   +- 2: compiler, {1}, ir
+- 3: backend, {2}, assembler
4: assembler, {3}, object
```
which tells us that the final output will be an *object* file.

### Preprocessor
After the initial parsing of the individual sources (charater mapping, comment stripping, line unwrapping, ... ) the preprocessor is the first real action on the code: all actions for preprocessor *directives* like `#include` and `#define` are performed which always result in a *single* code document which is the basis for the following compilation:
```pmans
/// ... // code from <stdlib.h> and nested #includes
extern void */*b6*/ calloc(...);
/// ... // code from <stdlib.h> and nested #includes
struct Grid {
  int nx;
  int ny;
  double *data;
};
typedef struct Grid GridType;
void grid_init( GridType *grid, int nx, int ny);
void grid_free( GridType *grid);
double * grid_at( GridType *grid, int x, int y);
void grid_init( GridType *grid, int nx, int ny) {
  grid->data = (double *)/*b6*/ calloc(nx * ny, sizeof(double));
  grid->nx = nx;
  grid->ny = ny;
}
void grid_free( GridType *grid) {
  free(grid->data);
  grid->nx = 0;
  grid->ny = 0;
}
double * grid_at( GridType *grid, int x, int y) {
  return &grid->data[x + grid->nx * y];
}
```
Note that the order of inclusion has consequences on this final code document.

### Compilation
The compilation process is performed according to the defined language standard. Using the C11 standard during compilation looks like
```pmans
/* compile: clang -std=c11 grid.c -c */ // using C11 language standard
```
As a intermediate result of this compilation, clang uses an intermediate represenation for all C-like languages called *abstract syntax tree* (AST) which can be viewed with

