//Class for fake process
//Author: Xinjing Wei

#include<stdlib.h>
#include "Process.h"

//Initialize a process
void Porcess_init(Process *p, char *name, char *currState, char *nextState, int t){
	p->pName=name;
	p->pStateCurr=currState;
	p->pStateNext=nextState;
	p->time=t;
}

//Get the process name
char *getPName(Process *p){
	if(p != NULL)
		return p->pName;
	else
		return "";
}

//Get the current state of the process
char *getPStateCurr(Process *p){
	if(p != NULL)
		return p->pStateCurr;
	else
		return "";
}

//Get the state that the process will transit to
char *getPStateNext(Process *p){
	if(p != NULL)
		return p->pStateNext;
	else
		return "";
}

//Get the time unit that the process has been staying in the current state
int getTime(Process *p){
	if(p != NULL)
		return p->time;
	else
		return 0;
}

int getTotalTime(Process *p){
	if(p != NULL)
		return p->totalTime;
	else
		return 0;
}
