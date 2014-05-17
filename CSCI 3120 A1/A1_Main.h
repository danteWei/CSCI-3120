#ifndef _A1_Main_h
#define _A1_Main_h

#include "list.h"
#include "Queue.h"

#define MAXLEN  (81)
#define MAXNAME (21)

typedef struct {
	Queue *New;
	Queue *Ready;
	Queue *Running;
	Queue *Blocked;
	Queue *Exit;

} SysState;

typedef struct {
	char name[MAXNAME];
	int lifeTime;	
} PCB;

void alarm_bells(int signal);
void printSysState();
void reConfig();
void exitAll();
void installHandlerInd(int *alrmSuc, int *urs1Suc, int *hupSuc, int *breakSuc);
void installHandlerCol(int *return_code);
int read_line(char line[], int len);


#endif
