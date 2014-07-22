#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructure.h"
#include "list.h"

#define MAXLINE (80) // Define the max length of a single input
#define MAXVIEW (3) // Define the max number of views

// Main function
int main(void){
	pthread_t controller; // Controller thread
	pthread_t Model; // Model thread
	pthread_t Views[MAXVIEW]={-1, -1, -1}; // Up to three view threads
	pthread_attr_t attrib; // thread attributes
	
	pthread_attr_init(&attrib);
	pthread_attr_setscope(&attrib, PTHREAD_SCOPE_SYSTEM);

	//create a shared memory segment for messages
	//int shmid=shmget(IPC_PRIVATE, sizeof(Messages), S_IRUSR | S_IWUSR);
	//create a package to share with threads
	//Package *pkg=(Package *) malloc(sizeof(Package));
	Package pkg;
	/*if (pkg == NULL){
		printf("Fail to allocate memory, program will terminate in 3 seconds.\n");
		sleep(3);
	}*/
	//initial values of the package
	int it=0, viewN=0;
	pkg.iteration=it;
	
	//pkg.published=(Grid *)list.tail->data;
	pkg.attrib=&attrib;
	pkg.viewNum=viewN;
	pkg.type="";
	pkg.outputFileName="";
	Messages *msgs=(Messages *) malloc(sizeof(Messages)); // Stores different messages
	if (msgs == NULL){
		printf("Fail to allocate memory, program will terminate in 3 seconds.\n");
		sleep(3);
	}
	pkg.msgs=msgs;
	//write initial messageReady
	messageReady *msgRD=(messageReady *) malloc(sizeof(messageReady)); //message ready, used to determine when to sync
	if (msgRD == NULL){
		printf("Fail to allocate memory, program will terminate in 3 seconds.\n");
		sleep(3);
	}

	msgRD->messageReady=0;
	msgRD->errorSync=1;

	pkg.msgs->msgRd=msgRD;
	pkg.Model=&Model;
	pkg.Views=Views;
	

	Legend *defaultLegend=(Legend *) malloc(sizeof(Legend)); // Define the default legend
	if (defaultLegend == NULL){
		printf("Fail to allocate memory, program will terminate in 3 seconds.\n");
		exit(EXIT_FAILURE);
	}
	defaultLegend->one='*';
	defaultLegend->zero='.';

	CntrlToView *cv=(CntrlToView *) malloc(sizeof(CntrlToView));
	if (cv == NULL){
		printf("Fail to allocate memory, program will terminate in 3 seconds.\n");
		exit(EXIT_FAILURE);
	}
	cv->viewNumber=-1;
	pkg.msgs->CVMsg=cv;

	pkg.msgs->CVMsg->legend=defaultLegend;

	//create Controller & Model threads
	pthread_create(&controller, &attrib, Controller, (void *)&pkg);
	pthread_create(&Model, &attrib, modelPoll, (void *)&pkg);

	//wait for controller to end
	pthread_join(controller, NULL);
	
	return 0;
}

//Controller function
void *Controller(void *p){
	Package *pkg=(Package *)p;
	CntrlToModel *CMMsg=(CntrlToModel *) malloc(sizeof(CntrlToModel));
	if (CMMsg == NULL){
		printf("Fail to allocate memory, program will terminate in 3 seconds.\n");
		sleep(3);
	}
	CMMsg->fileName="config.txt";
	pkg->msgs->CMMsg=CMMsg;
	pkg->msgs->msgRd->messageReady=1;

	//listen for input from keyboard
	read_input(pkg->Model, pkg->Views, pkg);
	//free all allocated memories
	if (freeMem(pkg) == 1)
		printf("All allocated memories are freed, program will exit.\n");
	else{
		printf("Fail to de-allocate memories, program will exit with failure.\n");
		sleep(2);
		exit(EXIT_FAILURE);
	}
	return NULL;
}

int freeMem(Package *pkg){
	int all_ok=0;
	if (pkg == NULL){
		all_ok=1;
	}
	else{
		if (pkg->msgs != NULL){
			if (pkg->msgs->msgRd != NULL)
				free(pkg->msgs->msgRd);
			if (pkg->msgs->CMMsg != NULL)	
				free(pkg->msgs->CMMsg);
			if (pkg->msgs->CVMsg != NULL)
				free(pkg->msgs->CVMsg);
			free(pkg->msgs);
		}
		
		all_ok=1;
	}
	return all_ok;
}

//read from standard input
void read_input(pthread_t *Model, pthread_t *Views, Package *pkg){
	char *line;
	char **inputs; //stores user inputs
	//allocate the pointer 
	inputs=(char **) malloc(sizeof(char *));
	if (inputs == NULL){
		printf("Unable to allocate memory, program will terminate in 3 seconds.\n");
		sleep(3);
		exit(EXIT_FAILURE);
	}	
	// read input
	while(1){
		// get input from keyboard
		fgets(line, MAXLINE-1, stdin);
		if (line == NULL || strcmp(line, "") == 0)
			continue;

		char *token=strtok(line, " ");
		int inputLen=0;
		//parse inputs
		while(token != NULL){
			*(inputs+inputLen)=token;
			token=strtok(NULL, " ");
			inputLen++;
		}
		if (inputLen == 0)
			continue;

		if (inputLen == 4){
			//start view <type> <file>
			if (strncmp(inputs[0], "start", 5) == 0 && strncmp(inputs[1], "view", 4) == 0){
				/* create view here */
			}
			//view <X> legend <legend> info
			else if (strncmp(inputs[0], "view", 4) == 0){
				int viewNum=atoi(inputs[1]);
				if (viewNum == 0){
					printf("Invalid input! Please check your input and try again.\n");
					line="";
					continue;
				}
				else if (viewNum > 3 && viewNum < 0){
					printf("Invalid view number! view number can only be in range (0, 3), please try again.\n");
					line="";
					continue;
				}
				else{
					/* parse legend info (single character legend)*/

				}
			}
			else{
				printf("Invalid input! Please try again.\n");
				line="";
				continue;
			}
		}
		//model <filename>
		else if (inputLen == 2){
			if (strncmp(inputs[0], "model", 5) == 0){
				/*re-read configuration file*/
				int *msgrd=&(pkg->msgs->msgRd->messageReady);
				//send a new message containning new file name to Model
				CntrlToModel *cmmsg=(CntrlToModel *) malloc(sizeof(CntrlToModel));
				cmmsg->fileName=inputs[1];
				if (pkg->msgs->CMMsg != NULL)
					free(pkg->msgs->CMMsg);
				pkg->msgs->CMMsg=cmmsg;
				//set message ready to 1
				*msgrd=1;
				//waiting for a message from Model
				while(*msgrd == 1);
				if (pkg->msgs->MCMsg == NULL){
					printf("Controller: No message received from Model.\n");
					continue;
				}
				else{
					pkg->iteration=pkg->msgs->MCMsg->iterationNum;
				}
			}
			else{
				printf("Invalid input! Please try again.\n");
				line="";
				continue;
			}
		}
		//end
		else if (inputLen == 1 && strncmp(inputs[0], "end", 3) == 0){
			if(exitProg(Model, Views) == 1){
					printf("All threads are terminated, system will free all allocated memory before exit.\n");
					break;
				}
				else{
					printf("Error when terminating threads, system will exit with failure in 3 seconds.\n");
					sleep(3);
					exit(EXIT_FAILURE);
				}
		}
		//view <X> legend <legend info>
		else if (inputLen > 4){
			if (strncmp(inputs[0], "view", 4) == 0 && strncmp(inputs[2], "legend", 6) == 0){
			int viewNum=atoi(inputs[1]);
				if (viewNum == 0){
					printf("Invalid input! Please check your input and try again.\n");
					line="";
					continue;
				}
				else if (viewNum > 3 && viewNum < 0){
					printf("Invalid view number! view number can only be in range (0, 3), please try again.\n");
					line="";
					continue;
				}
				else{
					/* parse legend info */
					if (inputLen == 5){
						printf("legend info must be either one character or 3 characters long.");
						printf("legend info will not be changed.\n");
					}
					if (inputLen >= 6){
						if (inputLen > 6)
							printf("The input legend info is longer than 3, all characters after 3 will be discarded.\n");
						Legend *tmp=(Legend *)NULL;
						Legend *newL=(Legend *) malloc(sizeof(Legend));
						if (pkg->msgs->CVMsg->legend != NULL)
							tmp=pkg->msgs->CVMsg->legend;
						else
							continue;
						int divide=atoi(inputs[4]);
						if (divide != 1){
							printf("Wrong legend info format, will use the default legend.\n");
							strcpy(&(newL->one), &(tmp->one));
							strcpy(&(newL->zero), &(tmp->zero));
						}
						else{
							strcpy(&(newL->one), &(inputs[3][0]));
							strcpy(&(newL->zero), &(inputs[5][0]));
						}
						pkg->msgs->CVMsg->legend=newL;
						if (tmp != NULL)
							free(tmp);
					}
				}
				
				pkg->msgs->CVMsg->viewNumber=viewNum;
				pkg->msgs->msgRd->messageReady=1;
				while(pkg->msgs->msgRd->messageReady == 1);
			}
		}
		else{
			printf("Invalid input! Please try again.\n");
			line="";
			continue;
		}



/*		if(sscanf(line, "%s %s %s %s", input1, input2, input3, input4) == 4){
			if(strncmp(input1, "start", 5) == 0){
				if(strncmp(input2, "view", 4) == 0){
					//create view here
					if(pkg->viewNum == 3)	
						printf("There are 3 views runnning, you cannot have more than 3 views.\n");
					else{
						pthread_create(&(*(Views+pkg->viewNum)), &(pkg->attrib), viewPoll, pkg);	
						pkg->viewNum+=1;	
					}
				}
				else
					printf("Invalid input.\n");
			}
			else	
				printf("Invalid input.\n");
		}
		else if (sscanf(line, "%s %s", input1, input2) == 2){
			if(strncmp(input1, "model", 5) == 0){
				//re-read configuration file
				pkg->msgs->CMMsg->fileName=input2;
				pkg->msgs->msgRd->messageReady=1;
			}
			else	
				printf("Invalid input.\n");
		}
		else if (sscanf(line, "%s %d %s %s", input1, &input5, input2, input3)){
			if(strncmp(input1, "view", 4) == 0){
				//legend info
			
			}
			else
				printf("Invalid input.\n");
		}
		//terminates Model & view threads
		else if (sscanf(line, "%s", input1) == 1){
			printf("YO\n");
			if(strcmp(input1, "end") == 0)
				if(exitProg(Model, Views) == 1){
					printf("All threads are terminated, system will free all allocated memory before exit.\n");
					break;
				}
				else{
					printf("Error when terminating threads, system will exit with failure in 3 seconds.\n");
					sleep(3);
					exit(EXIT_FAILURE);	
				}
			else
				printf("Invalid input!\n");

		}
		else
			printf("Incorrect input.\n");*/
	}
	free(inputs);
}

// Stop the program
int exitProg(pthread_t *model, pthread_t *views){
	int all_ok=1;
	//stops Model thread
	if(exitThread(model) != 1)
		all_ok=0;

	int i;
	//stops all view threads
	for (i=0; i<MAXVIEW; i++)
		if (*(views+i) != -1)
			if(exitThread(&(*(views+i))) != 1)
				all_ok=0;

	return all_ok;	
}

// Stop a thread
int exitThread(pthread_t *thread){
	int all_ok=1;
	if(pthread_cancel(*thread) != 0)
		all_ok=0;
	
	return all_ok;
}

