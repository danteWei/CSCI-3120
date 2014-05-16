//Main class of CSCI 3120 Assignment 1
//Author: Xinjing Wei

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include "A1_Main.h"
#include "list.h"
#include "Queue.h"
#include "Process.h"


#define MAXLEN 80
SysState *SYSSTATES;
Queue waitingQueue;
int TIMER;

//function for alarm interrupt
void alarm_bells(int singal){
	/*Start alarm*/
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
	char *line;
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
	alarm(timer);


	//Listen to the keyboard inputs and update the queues
	while(1){
		//Get line from keyboard with max string length
		fgets(line, MAXLEN, stdin);
		
		//Check if the input is read and is of the correct format
		if(line == NULL || (checkFormat(line) == -1)){
			printf("No input is read from keyboard or the input format is incorrect, please try it again.\n");
			continue;
		}


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

//Check the format of the input line
int checkFormat(char *line){
	int check=-1;
	
	/*Check format*/
	
	return check;
}
