/* file: grid.h */
#pragma once 
struct /*f4*/ Grid {
  int nx;
  int ny;
  double *data;
};
typedef struct Grid /*f8*/ GridType;
void /*b9*/ grid_init(/*f9*/ GridType *grid, int nx, int ny);
void /*b9*/ grid_free(/*f9*/ GridType *grid);
double */*b7*/ grid_at(/*f9*/ GridType *grid, int x, int y);
