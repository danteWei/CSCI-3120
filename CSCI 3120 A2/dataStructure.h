#ifndef _Grid_h_
#define _Grid_h_


typedef struct {
	int gridSize;
	char grid[gridSize][gridSize];
	int iterationNumber;
	char *status;
} Grid;

typedef struct {

} Messages;

typedef struct {

} CntrlToView;

typedef struct {

} CntrlToModel;

typedef struct {

} ModeltoView;


#endif
