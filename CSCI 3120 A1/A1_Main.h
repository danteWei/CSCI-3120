#ifndef _A1_Main_h
#define _A1_Main_h

#include "list.h"
#include "Queue.h"

typedef struct {
	Queue *New;
	Queue *Ready;
	Queue *Running;
	Queue *Blocked;
	Queue *Exit;

} SysState;

void alarm_bells(int signal);
void printSysState();
void reConfig();
void exitAll();
void installHandlerInd(int *alrmSuc, int *urs1Suc, int *hupSuc, int *breakSuc);
void installHandlerCol(int *return_code);
int read_line(char line[], int len);
int checkFormat(char *line);


#endif
