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

void read_stdin();
int removeRunning();
int moveNewToReady();
int moveToBlocked();
int moveBlockedToReady();
int alarm_helper();
void alarm_bells(int signal);
void printSysState();
void readConfig();
void exitAll();
void installHandlerInd(int *alrmSuc, int *urs1Suc, int *hupSuc, int *breakSuc);
void installHandlerCol(int *return_code);
int read_line(char line[], int len);
void printPCBTransition(PCB *pcb, List_t *fromL, List_t *toL);
int movePCB(List_t *fromL, List_t *toL);
void printPCB(PCB *p);
void printState(List_t *list);

#endif
