#include <stdlib.h>
#include "GameOfLife.h"
#include <unistd.h>

//returns 1 if iteration number is increased, returns 0 otherwise
int viewSync(int it, int last){
	if (last == it)
		return 0;
	
	return 1;
};

void *viewPoll(void *p){
	Package *pkg=(Package *)p;
	int lastPrint=-1;
	//stores view info so that it won't change with pkg
	char *type=pkg->type;
	Legend *lgd=pkg->msgs->CVMsg->legend;
	char *outputFileName=pkg->outputFileName;
	int viewN=pkg->viewNum;
	//poll changes
	while(1){
		int *msgrd=&(pkg->msgs->msgRd->messageReady);
		if (*msgrd == 1){
			if (pkg->msgs->CVMsg->viewNumber == viewN){
				lgd=pkg->msgs->CVMsg->legend;
			}
			*msgrd=0;
		}
		if (pkg->published == NULL)
			continue;

		Grid *g=pkg->published;
		if (strcmp(type, ""))
			continue;
		else if (strcmp(type, "full")){
			/* print full view to the console */
			printf("Iteration number: %d\n", pkg->iteration);
			int i, j;
			for (i=0; i<g->len; i++){
				for (j=0; j<g->width; j++){
					if (g->grid[i][j] == 0)
						printf("%c ", lgd->zero);
					else
						printf("%c ", lgd->one);
				}
				printf("\n");
			}
			
		}
		else if (strcmp(type, "summary")){
			/* print summary view to the console */
			int count=0, i, j;
			//count number of 1's in the grid
			for(i=0; i<g->len; i++){
				for(j=0; j<g->width; j++)
					if (g->grid[i][j] == 1)
						count++;
			}
			printf("number of cells alive: %d\n\n", count);
		}
		else
			continue;
		
		//write information to a file
		if (write_to_file(g, type, pkg->iteration, outputFileName, lgd, &lastPrint, &(pkg->printed)) == 0)
			printf("Unable to write the grid to file \"%s\".\n", outputFileName);
		sleep(1);
	}
	return NULL;
}



int write_to_file(Grid *g, char *type, int it, char *outputFileName, Legend *l, int *last, int *printed){
	//check if the iteration number is increased or not
	if (viewSync(it, *last) == 0)
		return 1;
	//if increased, copy information to the file
	FILE *fp=0;

	//open, or possibly create the file for appending
	fopen(outputFileName, "ab+");
	//check if the file is opened
	if (fp == 0){
		printf("Unable to open/create file.\n");
		return 0;
	}
	//print iteration number
	fprintf(fp, "Iteration number: %d\n", it);
	//if the type is full
	if (strcmp(type, "full")){
		//append grid to the file
		int i, j;
		for(i=0; i<g->len; i++){
			for(j=0; j<g->width; j++){
				if (g->grid[i][j] == 0)
					fprintf(fp, "%c ", l->zero);
				else
					fprintf(fp, "%c ", l->one);
			}
			fprintf(fp, "\n");
		}
	}
	//if the type is summary
	else{
		int count=0, i, j;
		//count number of 1's in the grid
		for(i=0; i<g->len; i++){
			for(j=0; j<g->width; j++)
				if (g->grid[i][j] == 1)
					count++;
		}
		fprintf(fp, "number of cells alive: %d\n", count);
	}
	fprintf(fp, "\n\n\n");
	//update printed
	*printed+=1;
	fclose(fp);

	//before return, update lastPrint
	*last=it;
	return 1;
}
