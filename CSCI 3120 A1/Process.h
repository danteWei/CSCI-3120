//Header file of Process
//Author: Xinjing Wei

#ifndef _Process_h_
#define _Process_h_

#include<stdlib.h>

//Define the Process structure
typedef struct {
	char *pName;
	char *pStateCurr;
	char *pStateNext;
	int  time;
	int totalTime;
} Process;

int Process_init(Process *p);
char *getPName(Process *p);
char *getPStateCurr(Process *p);
char *getPStateNext(Process *p);
int getTime(Process *p);
int getTotalTime(Process *p);

#endif
