#ifndef _grid_h_
#define _grid_h_

#include <semaphore.h>
#include "common.h"

typedef struct {
  char letter;
  sem_t cell_lock;	//add lock to every cell
} gridcell;

typedef struct {
  int rows;
  int cols;
  gridcell **grid;
  int x_min, x_max, y_min, y_max;
} grid;

#define GRID_LOAD_PERCENT (20)
#define EMPTY_GRID_CELL '.'

boolean grid_init( grid *the_grid, int capacity );
void grid_free( grid *the_grid );
void grid_getdimensions( grid *the_grid, int *rows, int *cols );
void grid_print( grid *the_grid, FILE *outfile, boolean solution_only );
boolean grid_wordfits( grid *the_grid, char *word, int x, int y, int xdir, int ydir, int *count );
void grid_placeword( grid *the_grid, char *word, int x, int y, int xdir, int ydir );
void grid_shrink( grid *the_grid );

void unlock_cells(grid *the_grid, int x, int y, int xdir, int ydir, int i);

#endif

