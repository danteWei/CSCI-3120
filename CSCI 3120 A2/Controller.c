#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "dataStructure.h"
#include "Model.h"
#include "View.h"

// Define the max length of a single input
#define MAXLINE 80
#define MAXVIEW 3

// Main function
int main(void){
	pthread_t Model;
	pthread_t Views[MAXVIEW];
	pthread_attr_t attrib;


	//listen for input from keyboard
	while(1)
		read_input();


	return 0;
}

//read from standard input
void read_input(){
	char line[MAXLINE]; // stores the input from stdin
	char *cmd; // stores the command
	int viewNum; // stores the view number
	char *option; // stores the option of the command
	char *fileName; // stores the file name
	char *legendInfo; // stores the legend information

	// get input from keyboard
	fgets(line, MAXLINE-1, stdin);
	
	// read input
	if(sscanf(line, "%s %s %s", cmd, option, fileName) == 3){
	
	}
	else if (sscanf(line, "%s %s", cmd, fileName) == 2){
	
	}
	else if (sscanf(line, "%s %d %s %s", cmd, &viewNum, option, legendInfo)){
	
	}
	else if (sscanf(line, "%s", cmd) == 1){
		if(strcmp(tolower(cmd), "end") == 0)
			if(exitProg(Model, Views) == 1){
				printf("All threads are terminated, system will exit in 3 seconds.\n");
				sleep(3);
				exit(EXIT_SUCCESS);
			}		
	}
	else
		printf("Incorrect input");

}

// Stop the program
int exitProg(thread_t model, thread_t * views[MAXVIEW]){
	int all_ok=1;
	
	if(exitThread(model) != 1)
		all_ok=0;

	int i;
	for (i=0; i<MAXVIEW; i++)
		if(exitThread(views[i]) != 1)
			all_ok=0;

	return all_ok;	
}

// Stop a thread
int exitThread(pthread_t thread){
	int all_ok=1;
	if(pthread_cancel(thread) != 0)
		all_ok=0;
	
	return all_ok;
}
