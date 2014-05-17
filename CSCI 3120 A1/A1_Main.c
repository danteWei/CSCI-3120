//Main class of CSCI 3120 Assignment 1
//Some of the ideas come from the codes written by Michael McAllister
//Author: Xinjing Wei

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include "A1_Main.h"
#include "list.h"
#include "Queue.h"
#include "Process.h"


#define MAXLEN (81)
#define MAXNAME (21)

SysState *SYSSTATES;
Queue waitingQueue;
int TIMER=0;

//function for alarm interrupt
void alarm_bells(int singal){
	/*Start alarm*/
	alarm(TIMER);
}

//function for urs1 interrupt
void printSysState(){
	/*Print information of all queues*/
}

//function for hup interrupt
void reConfig(){
	FILE *fp=0;
	fp=fopen("config.txt", "r");
	if(fp == 0){
		printf("Fail to open the file, please check the file name! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}

	/*Read the new timer*/
	


	//Close the file
	if(fclose(fp) != 0){
		printf("Fail to close the file! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}
}

//function for break interrupt
void exitAll(){
	//Destroy all Queues
	Queue_clear(SYSSTATES->New);
	Queue_clear(SYSSTATES->Ready);
	Queue_clear(SYSSTATES->Running);
	Queue_clear(SYSSTATES->Blocked);
	Queue_clear(SYSSTATES->Exit);

	/*Free other memory allocations*/


	printf("All memory space are freed, program will terminate in 3 seconds");
	sleep(3);
	//Exit successfullly
	exit(EXIT_SUCCESS);

}

//install the handlers independently, one install failure will not stop the whole program
void installHandlerInd(int *alrmSuc, int *urs1Suc, int *hupSuc, int *breakSuc){
	char errInstallMsg[]="Unbale to install the ";
	if(signal(SIGALRM, alarm_bells) == SIG_ERR){
		printf("%salarm handler!\n", errInstallMsg);
		*alrmSuc=-1;
	}
	if(signal(SIGUSR1, printSysState) == SIG_ERR){
		printf("%surs1 handler!\n", errInstallMsg);
		*urs1Suc=-1;
	}
	if(signal(SIGHUP, reConfig) == SIG_ERR){
		printf("%shup handler!\n", errInstallMsg);
		*hupSuc=-1;
	}
	if(signal(SIGINT, exitAll) == SIG_ERR){
		printf("%sbreak handler!\n", errInstallMsg);
		*breakSuc=-1;
	}
}

//install the handlers collectively, one failture will stop the whole program
void installHandlerCol(int *return_code){
	if(signal(SIGALRM, alarm_bells) == SIG_ERR || signal(SIGUSR1, printSysState) == SIG_ERR || signal(SIGHUP, reConfig) == SIG_ERR || signal(SIGINT, exitAll) == SIG_ERR){
		printf("Unable to install handler!\n");
		*return_code=-1;
	}
}

//Main function
int main(int argc, char **argv){
	int return_code=0;
	int timer;
	char line[MAXLEN];
	char name[MAXLEN];
	int lifeTime, runTime;
	PCB *pcb;

	//For checking install status
	int alrmSuc=1, urs1Suc=1, hupSuc=1, breakSuc=1;

	//Install the handlers
	installHandlerInd(&alrmSuc, &urs1Suc, &hupSuc, &breakSuc);

	//If all installation fail, exit the program with error code -1
	if(alrmSuc == -1 && urs1Suc == -1 && hupSuc == -1 && breakSuc == -1){
		printf("All installatioin failed! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}

	if(alrmSuc == 1 && urs1Suc == 1 && hupSuc == 1 && breakSuc == 1)
		printf("All interrupt handlers are installed successfully, the program is good to go.\n");

	//Open configuration file
	FILE *fp=0;
	fp=fopen("config.txt", "r");
	
	//check if something wrong when opening the file
	if(fp == 0){
		printf("Fail to open the file! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}
	
	//Initialize system states queues
	Queue *newQ, *readyQ, *runQ, *blockedQ, *exitQ;
	int newCheck=Queue_init(newQ, "new");
	int runCheck=Queue_init(runQ, "run");
	int readyCheck=Queue_init(readyQ, "ready");
	int blockedCheck=Queue_init(blockedQ, "blocked");
	int exitCheck=Queue_init(exitQ, "exit");
	
	if(!(newCheck && runCheck && readyCheck && blockedCheck && exitCheck)){
		printf("Fail to initilize the state queues, program will terminate in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}

	SYSSTATES->New=newQ;
	SYSSTATES->Ready=readyQ;
	SYSSTATES->Running=runQ;
	SYSSTATES->Blocked=blockedQ;
	SYSSTATES->Exit=exitQ;

	

	/*
	 *Codes contniue here
	 */
	

	//Close file
	int file_close_code=fclose(fp);
	if(file_close_code != 0){
		printf("Fail to close the file! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}

	//Set the alarm interrupt going
	if(TIMER > 0)
		alarm(TIMER);


	//Listen to the keyboard inputs and update the queues
	//The idea of most of the codes in the while loop are from parse.c written by Michael McAllister
	while(1){
		//Get line from keyboard with max string length
		fgets(line, MAXLEN-1, stdin);
		
		//Check if the input is read and is of the correct format
		if(line == NULL){
			printf("No input is read from keyboard or the input format is incorrect, please try it again.\n");
			continue;
		}

		if(sscanf(line, "%s %d %d", name, &lifeTime, &runTime) == 3){
			pcb=(PCB *) malloc(sizeof(PCB));
			if(pcb != NULL){
				strncpy(pcb->name, name, MAXNAME-1);
				//Terminates the string
				pcb->name[MAXNAME-1]='\0';
				pcb->lifeTime=lifeTime;

				//If the process was not added to the queue
				if(Queue_add_tail((void *) pcb, newQ) == 0)
					printf("Error in adding the process into the queue.\n");
			}
			else
				printf("System out of memory.\n");

		}
		else
			printf("Incorrect number of inputs!\n");


	}

	



	return return_code;
}

//Read inputs from keyboard
int read_line(char line[], int len) {
  int ch, i = 0;

  while ((ch = getchar()) != '\n') 
    if (i < len) 
      line[i++] = ch;
  
  line[i] = '\0';

  return i;
}

