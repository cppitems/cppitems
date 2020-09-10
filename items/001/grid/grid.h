#pragma once
struct Grid {
  int nx;
  int ny;
  double *data;
};
typedef struct Grid GridType;
void grid_init(GridType *grid);
void grid_free(GridType *grid);
double *grid_at(GridType *grid, int x, int y);
