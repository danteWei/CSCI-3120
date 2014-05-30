#ifndef _Grid_h_
#define _Grid_h_

#define GRIDSIZE 4;

typedef struct {
	char grid[GRIDSIZE][GRIDSIZE];
	int iterationNumber;
	char *status;
} Grid;


#endif
