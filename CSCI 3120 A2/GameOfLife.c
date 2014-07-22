#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructure.h"


int game(Grid *g, int ***array){
	//assign a pointer to the preceding grid, and other information
	int **grid=g->grid;
	int len=g->len;
	int width=g->width;
	int **resultGrid=(int **)malloc(sizeof(int *)*len); //new grid
	int i, j;
	//check if the malloc succeeded
	if(resultGrid == NULL)
		return 0;

	//make a copy of the old grid
	for(i=0; i<len; i++){
		resultGrid[i]=(int *)malloc(sizeof(int)*width);

		//check if the malloc succeeded
		if(resultGrid[i] == NULL)
			return 0;
		for(j=0; j<width; j++)
			resultGrid[i][j]=grid[i][j];
	}
	int count=0; //count the alive neighbours
	//Game start
	for (i=0; i<len; i++)
		for (j=0; j<width; j++){
			//check the neighbours that are above or below the current cell
			//if the cell is not in the first row
			if(i != 0){
				//if the cell is not at the top-left
				if (j != 0)
					if (grid[i-1][j-1] == 1)
						count++;
				if (grid[i-1][j] == 1)
					count++;
				//if the cell is not at the top-right
				if (j != (width-1))
					if (grid[i-1][j+1] == 1)
						count++;
			}
			//if the cell is not in the last row
			if (i != (len-1)){
				//if the cell is not at the bottom-right
				if (j != (width-1))
					if (grid[i+1][j+1] == 1)
						count++;
				if (grid[i+1][j] == 1)
					count++;
				//if the cell is not at the bottom-left
				if (j != 0)
					if(grid[i+1][j-1])
						count++;
			}
			//check the neighbours that are in the same row
			//if the cell is not in the first column
			if (j != 0)
				if (grid[i][j-1] == 1)
					count++;
			//if the cell is not in the last column
			if (j != (width-1))
				if(grid[i][j+1] == 1)
					count++;
			//update grid
			if (grid[i][j] == 1){
				if (count > 3 || count < 2)
					resultGrid[i][j]=0;
				else
					resultGrid[i][j]=1;
			}
			else
				if (count == 3)
					resultGrid[i][j]=1;
			//reset count
			count=0;
		}
	//assign the pointer to the resulting grid
	*array=resultGrid;
	return 1;
}

