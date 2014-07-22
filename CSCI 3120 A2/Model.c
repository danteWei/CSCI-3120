#include <stdlib.h>
#include <stdio.h>
#include "dataStructure.h"
#include <unistd.h>

void *modelPoll(void *p){
	int numView=0;
	char *filename="";
	int newFile=0;
	int itNum=0;
	Grid *work, *published;
	int init=1;
	CntrlToModel *m;
	while(1){
		Package *pkg=(Package *)p;
		char *tempFileN="";
		int tempNumView=0;
		Grid *new=(Grid *) malloc(sizeof(Grid));
		if (pkg->msgs->msgRd->messageReady == 1){
			//determine the type of message
			if (pkg->msgs->CMMsg != NULL){
				if (m != NULL){
					//a new message from controller to model
					if (m != pkg->msgs->CMMsg){
						m=pkg->msgs->CMMsg;
					}
					//no message for model
					else{
						//wait for views to sync
						while(pkg->msgs->msgRd->messageReady == 1);
						continue;
					}
				}
				
				//make a copy of values
				tempNumView=pkg->viewNum;
				tempFileN=pkg->msgs->CMMsg->fileName;
			}
			else{
				printf("Model: No message from Controller is detected.\n");
				pkg->msgs->msgRd->messageReady=0;
				continue;
			}
			//if being nodified a new view
			if (tempNumView > numView){
				//update number of views
				numView=tempNumView;
				//put next grid iteration number in pkg
				ModelToCntrl *MCMsg=(ModelToCntrl *) malloc(sizeof(ModelToCntrl));
				MCMsg->iterationNum=itNum+1;
				if (pkg->msgs->MCMsg != NULL)
					free(pkg->msgs->MCMsg);
				pkg->msgs->MCMsg=MCMsg;
			}
			//if being nodified of a new file name
			if (strcmp(filename, tempFileN) != 0){
				filename=tempFileN;
				newFile=1;
			}
			else
				printf("The file \"%s\" has already been read before, Model will ignore this message\n", filename);
			
			//set "message ready to 0"
			pkg->msgs->msgRd->messageReady=0;
		}

		//if I have a new file name
		if (newFile == 1){
			//load the new file name as the working grid
			Grid *tmp=(Grid *)NULL;
			if (work != NULL)
				tmp=work;
			if (read_file(filename, work, &itNum, &init) == 0){
				printf("Fail to read from file \"%s\", program will terminate in 3 seconds.\n", filename);
				sleep(3);
				exit(EXIT_FAILURE);
			}
			
			if (tmp != NULL)
				free(tmp);
		}
		else{
			//calculate next iteration of grid
			itNum++;
			if (work == NULL){
				printf("No working grid detected!\n");
				continue;
			}
			//make the old published grid the working grid
			Grid *tmp=(Grid *)NULL;
			if (published != NULL){
				if (work != NULL)
					tmp=work;
				work=published;
				work->status="work";
			}
			
			new->len=published->len;
			new->width=published->width;
			new->status="published";
			//game
			game(work, &(new->grid));
			new->iterationNumber=(published->iterationNumber+1);
			if (tmp != NULL)
				free(tmp);
			
		}
		//poll for all the viewers to be done with the ealier published grid
		while (numView > pkg->printed);
		pkg->printed=0;
		//make the calculated grid as published
		published=new;
		//increment iteration number
		pkg->iteration+=1;
		pkg->published=published;
		//delay for 2 seconds
		sleep(2);
	}
	return NULL;
}

int read_file(char *fileName, Grid *g, int *itN, int *init){
	FILE *fp=0;
	fp=fopen(fileName, "r");
	if (fp == 0){
		printf("no such file\n");
		return 0;
	}
	//reset iteration number
	*itN=0;
	int length, width;
	char line[MAXLINE];
	Grid *grid=(Grid *) malloc(sizeof(Grid));
	if (*init == 0)
		grid->status="work";
	else{
		grid->status="published";
		*init=0;
	}
	//get len & width
	fgets(line, MAXLINE, fp);
	if (sscanf(line, "%d %d", &length, &width) == 2){
		grid->len=length;
		grid->width=width;
	}
	else{
		printf("file format is wrong.\n");
		return 0;
	}
	//declare a 2-D array
	int **array=(int **) malloc(sizeof(int *)*length);
	int i;
	for(i=0; i<width; i++){
		array[i]=(int *) malloc(sizeof(int)*width);
	}
	int row=0;
	while (fgets(line, MAXLINE, fp) != NULL && row < length){
		char *token=strtok(line, " ");
		for(i=0; i<width && token != NULL; i++){
			char *num=token;
			array[row][i]=atoi(num);
			token=strtok(NULL, " ");
		}
		row++;
	}
	grid->iterationNumber=*itN;
	grid->grid=array;
	g=grid;
	fclose(fp);
	return 1;
}
