#ifndef _dataStructure_h_
#define _dataStructure_h_

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define MAXLINE (80)
#define MAXVIEW (3)

//data structures
typedef struct {
	char one;
	char zero;
} Legend;


typedef struct {
	int messageReady;
	int errorSync;
} messageReady;

typedef struct {
	int len;
	int width;
	int **grid;
	int iterationNumber;
	char *status;
} Grid;

typedef struct {
	char *fileName;
} CntrlToModel;

typedef struct {
	int iterationNum;
} ModelToCntrl;

typedef struct {
	Legend *legend;
	int viewNumber;
} CntrlToView;

typedef struct {
	CntrlToModel *CMMsg;
	ModelToCntrl *MCMsg;
	CntrlToView *CVMsg;
	messageReady *msgRd;
} Messages;


//This is a struct that is used to share informations across threads
typedef struct {
	Messages *msgs;
	int iteration;
	pthread_attr_t *attrib;
	int viewNum;
	pthread_t *Model;
	pthread_t *Views;
	Grid *published;
	char *type;
	char *outputFileName;
	
	int printed;
} Package;


//Model header file
void *modelPoll(void *p); //function for Model thread, sync every 2 seconds
int read_file(char *fileName, Grid *g, int *itN, int *init); //create a grid from a file


//View header file
int viewSync(int it, int last); //synchromnizes View threads
void *viewPoll(void *p); //function for View threads, sync ecery second
int write_to_file(Grid *g, char *type, int it, char *outputFileName, Legend *l, int *last, int *printed); // write info to the file


//Controller header file
void read_input(pthread_t *Model, pthread_t *Views, Package *pkg); //read user inputs
int exitProg(pthread_t *model, pthread_t *views); //terminates all threads except for Controller
int exitThread(pthread_t *thread); //terminates one thread
void *Controller(void *p); //function for Controller thread
int freeMem(Package *pkg); //free all memories before termination


//Game of Life header file
int game(Grid *g, int ***array); //calculate the next iteration

#endif
