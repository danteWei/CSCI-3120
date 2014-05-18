//Main class of CSCI 3120 Assignment 1
//Some of the ideas come from the codes written by Michael McAllister
//Author: Xinjing Wei


/*I can build a structure called Queue based on the structure List_t, but I chose not to
*The reason is that I think it is unnecessary to do that, 
*because if I do, I just need to wrap List_t within Queue, 
*which is, obviously, unnecessary.
*/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include "A1_Main.h"
#include "list.h"


#define MAXLEN (81)
#define MAXNAME (21)

SysState SYSSTATES;



int TIMER=0;
int runStateClock=0;
int runningProcessLifetime=0;
int blockedClock=5;

//move pcbs to the targeted queues
int alarm_helper(){
	int all_ok=1;
	if(List_size(SYSSTATES.Running) != 0 && (runStateClock == 0 || runningProcessLifetime == 0)){
		PCB *p=NULL;
		if(List_head_info(SYSSTATES.Running, (void *) &p) == 1){
			if((*p).lifeTime == 0)
				movePCB(SYSSTATES.Running, SYSSTATES.Exit);
			
			else
				movePCB(SYSSTATES.Running, SYSSTATES.Blocked);
			p=NULL;
		}
		else{
			printf("Error retrieving head data. ");
			all_ok=0;
		}	
	}
	
	//Move all processes in the new queue to the ready queue
	if(List_size(SYSSTATES.New) != 0){
		List_node_t *curr=(SYSSTATES.New)->head;
		while(curr != NULL){
			curr=curr->next;
			movePCB(SYSSTATES.New, SYSSTATES.Ready);
		}
		//printf("size of new %d; size of ready %d\n", List_size(SYSSTATES.New), List_size(SYSSTATES.Ready));
	}

	return all_ok;
}

//function for alarm interrupt
void alarm_bells(int singal){
	/*Start alarm*/
	printf("alarm start\n");
	
	//decrement clocks
	if(runStateClock != 0 && runningProcessLifetime != 0){
		runStateClock--;
		runningProcessLifetime--;
	}
	else{ //If CPU has processed enough time for a process, move it to the blocked queue
		if(List_size(SYSSTATES.Ready) != 0){
			PCB *p=NULL;
			if(List_head_info(SYSSTATES.Ready, (void *) &p)){
				runStateClock=p->runningTime;
				runningProcessLifetime=p->lifeTime;
				//The process that has finished running is not removed from the running queue
				//but it will be in alaim_helper function
				movePCB(SYSSTATES.Ready, SYSSTATES.Running);
			}
			else{
				printf("Error retrieving data. Program will terminate in 3 seconds.\n");
				sleep(3);
				exit(EXIT_FAILURE);
			}
		}
	}

	if(blockedClock != 0)
		blockedClock--;
	else{ //If a process has stay at the head of the blocked queue for 5 time unit, move it to the ready queue
		if(List_size(SYSSTATES.Blocked) != 0){
			movePCB(SYSSTATES.Blocked, SYSSTATES.Ready);

		}
		blockedClock=5;
	}

	//call the helper function
	if(alarm_helper() == 0){
		printf("Program will terminate in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}


	alarm(TIMER);
}

//function for urs1 interrupt
void printSysState(){
	/*Print information of all queues*/
}

//function for hup interrupt
void readConfig(){
	//Open configuration file
	FILE *fp=0;
	fp=fopen("config.txt", "r");

	//check if something wrong when opening the file
	if(fp == 0){
		printf("Fail to open the file, please check the file name! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}

	//Read timer from config.txt
	char timerLine[MAXLEN];
	if(fgets(timerLine, MAXLEN-1, fp) != NULL)
		TIMER=timerLine[6]-'0';

	//close file
	if(fclose(fp) != 0){
		printf("Fail to close the file! Program will exit in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}
}

//function for break interrupt
void exitAll(){
	//Destroy all Queues
	List_destroy(SYSSTATES.New);
	List_destroy(SYSSTATES.Ready);
	List_destroy(SYSSTATES.Running);
	List_destroy(SYSSTATES.Blocked);
	List_destroy(SYSSTATES.Exit);

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
	if(signal(SIGHUP, readConfig) == SIG_ERR){
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
	if(signal(SIGALRM, alarm_bells) == SIG_ERR || signal(SIGUSR1, printSysState) == SIG_ERR || signal(SIGHUP, readConfig) == SIG_ERR || signal(SIGINT, exitAll) == SIG_ERR){
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

	//update TIMER
	readConfig();



	//Initialize system states queues
	List_t newQ, readyQ, runQ, blockedQ, exitQ;
	int newCheck=List_init(&newQ, "new");
	int runCheck=List_init(&runQ, "run");
	int readyCheck=List_init(&readyQ, "ready");
	int blockedCheck=List_init(&blockedQ, "blocked");
	int exitCheck=List_init(&exitQ, "exit");
	
	if(!(newCheck && runCheck && readyCheck && blockedCheck && exitCheck)){
		printf("Fail to initilize the state queues, program will terminate in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}

	SYSSTATES.New=&newQ;
	SYSSTATES.Ready=&readyQ;
	SYSSTATES.Running=&runQ;
	SYSSTATES.Blocked=&blockedQ;
	SYSSTATES.Exit=&exitQ;
	
	/*
	 *Codes contniue here
	 */


	//Set the alarm interrupt going
	if(TIMER > 0)
		alarm(TIMER);
	else
		printf("No timer is set or the timer is set to 0.\n");

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
			//printf("Process name: %s; Life time: %d; Run state time: %d\n", name, lifeTime, runTime);
			//If the input is incorrect, promote user to input again
			while((lifeTime < runTime || lifeTime < 0 || runTime <= 0)){
				if(lifeTime < runTime)
					printf("Process %s\'s lifetime cannot be less than its run time!\n", name);
				else if(lifeTime < 0)
					printf("Process %s\'s lifetime cannot be less than 0!\n", name);
				else if(runTime <= 0)
					printf("Process %s\'s run time cannot be less than or equal to 0!\n", name);
				printf("Please try again with this format: <processname> <lifetime> <run time>\n");
				sscanf(line, "%s %d %d", name, &lifeTime, &runTime);
			}
			pcb=(PCB *) malloc(sizeof(PCB));
			if(pcb != NULL){
				strncpy(pcb->name, name, MAXNAME-1);
				//Terminates the string
				pcb->name[MAXNAME-1]='\0';
				pcb->lifeTime=lifeTime;
				pcb->runningTime=runTime;
				
				//If the process was not added to the queue
				if(List_add_tail( SYSSTATES.New, (void *) pcb ) == 0)
					printf("Error in adding the process into the queue.\n");
			}
			else
				printf("System out of memory.\n");

		}
		else
			printf("Incorrect order of inputs!\n");


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

//Move a pcb from one queue to another
int movePCB(List_t *fromL, List_t *toL){
	int all_ok=0;
	PCB *temp=NULL;	
	int removeOK=List_remove_head(fromL, (void *)&temp);
	int addOK=List_add_tail(toL, (void *)temp);
	if(removeOK == 1 && addOK == 1){
		all_ok=1;
		printPCBTransition(temp, fromL, toL);
	}
	return all_ok;

}

//Print the information of a process that has been moved from one state to another
void printPCBTransition(PCB *pcb, List_t *fromL, List_t *toL){
	printf("%s transition from %s to %s\n", pcb->name, fromL->name, toL->name);
}
