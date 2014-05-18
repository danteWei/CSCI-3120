#ifndef _A1_Main_h
#define _A1_Main_h

#include "list.h"

#define MAXLEN  (81)
#define MAXNAME (21)

typedef struct {
	List_t *New;
	List_t *Ready;
	List_t *Running;
	List_t *Blocked;
	List_t *Exit;

} SysState;

typedef struct {
	char name[MAXNAME];
	int lifeTime;	
	int runningTime;
} PCB;

int alarm_helper();
void alarm_bells(int signal);
void printSysState();
void reConfig();
void exitAll();
void installHandlerInd(int *alrmSuc, int *urs1Suc, int *hupSuc, int *breakSuc);
void installHandlerCol(int *return_code);
int read_line(char line[], int len);
void printPCB(PCB *pcb);
int movePCB(List_t *fromL, List_t *toL);


#endif
