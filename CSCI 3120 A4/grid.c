#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <semaphore.h>
#include "common.h"
#include "grid.h"

/* We manage a grid of letters that represent a word search puzzle.  The
   puzzle is a two-dimentional grid.

   The grid is designed for use in creating the puzzles.  To that end
   we create a grid that is much larger than needed, ask the users of
   the grid to fill in words near the centre of the grid, and then trim
   the unused part of the grid on demand.

   The grid's rows and cols fields represent the absolute size of the grid.
   Those are the "much larger" dimensions.

   The shrunken grid is the range [x_min...x_max][y_min...y_max].
*/


/* Initialize a grid with both the basic parameters like size as well
   as the space for the too-large grid.

   Return True if the space was allocated and False if a problem arose during
   the initization and nobody should use the grid. */

/* Unlock every cells from the indicated position (x, y) to (x+i*xdir, y+i*ydir) along the direction (xdir, ydir) */

void unlock_cells(grid *the_grid, int x, int y, int xdir, int ydir, int i){
  int j;
  for (j=0; j<i; j++){
    int lock;
    if (sem_getvalue(&(the_grid->grid[x+j*xdir][y+j*ydir].cell_lock), &lock) == 0)
      if (lock <= 0)
        if (sem_post(&(the_grid->grid[x+j*xdir][y+j*ydir].cell_lock)) != 0)
          perror(strerror(errno));
  }
}

boolean
grid_init( grid *the_grid, int capacity ) {
  boolean init = False;
  int totalcells;
  int i, j;

  if ((the_grid != NULL) && (capacity > 0)) {
    /* To determine the size of the grid, we have an expected loading factor
       of the grid...basically what percentage of the grid is likely to be
       filled with actual letters from words rather than filler. Choose
       the total number of cells in the grid to reach that load factor.

       After that, assume that we'll fill those cells with a square grid. */

    totalcells = (int)(capacity / (GRID_LOAD_PERCENT/100.0));

    the_grid->rows = (int)(sqrt( totalcells * 1.0 )) + 1;
    the_grid->cols = the_grid->rows;

    /* Until we fill the grid, the shrunk grid should be the whole grid. */

    the_grid->x_min = 0;
    the_grid->x_max = the_grid->rows;
    the_grid->y_min = 0;
    the_grid->y_max = the_grid->cols;

    /* Create the space for the grid. */

    the_grid->grid = (gridcell **) malloc(the_grid->cols * sizeof( gridcell * )  );

    if (the_grid->grid != NULL) {
      /* Set up the 2d array structure. */

      init = True;
      for (i = 0; (i < the_grid->cols) && init; i++) {
        the_grid->grid[i] = (gridcell *) malloc( the_grid->rows * sizeof( gridcell ) );
        if (the_grid->grid[i] != NULL) {
          /* Initialize all those cells. */

          for (j = 0; j < the_grid->rows; j++) {
            the_grid->grid[i][j].letter = EMPTY_GRID_CELL;
            if (sem_init(&(the_grid->grid[i][j].cell_lock), 0, 1) == -1){
              init = False;
              break;
            }
          }
        } else {
          init = False;
        }
      }
    }
  }

  return init;
}

/* Release all the space allocated to the grid.  The dynamic space is all
   in the grid cells. 

   Leave the grid fields set as if there is nothing in the grid.
*/

void 
grid_free( grid *the_grid ) {
  int i, j;

  if (the_grid != NULL) {
    /* Free up the core of the grid. */

    for (i = 0; i < the_grid->cols; i++) {
      if (the_grid->grid[i] != NULL)
        
        /* Destroy all locks */

        for (j=0; j<the_grid->rows; j++)
          sem_destroy(&(the_grid->grid[i][j].cell_lock));
        free( the_grid->grid[i] );
    }
    free( the_grid->grid );

    /* Reset the grid struture to something safe. */

    the_grid->grid = NULL;
    the_grid->rows = 0;
    the_grid->cols = 0;
    the_grid->x_min = 0;
    the_grid->x_max = 0;
    the_grid->y_min = 0;
    the_grid->y_max = 0;
  }
}

/* Report the current maximum sizes of the grid that we're using. */

void
grid_getdimensions( grid *the_grid, int *rows, int *cols ) {
  *rows = 0;
  *cols = 0;

  if (the_grid != NULL) {
    *rows = the_grid->rows;
    *cols = the_grid->cols;
  }
}

/* Print the contents of the grid.  If "solution_only" is False then
   every cell of the grid is printed with a letter.  If it is True then
   we only print letters in the cells used for a word and the unused
   cells are printed with the "empty" character. */

void
grid_print( grid *the_grid, FILE *outfile, boolean solution_only ) {
  int i, j;

  if ((the_grid != NULL) && (outfile != NULL)) {
    /* Report the overall grid size for information. */

    fprintf (outfile, "Grid dimensions: %d x %d\n\n", the_grid->x_max - the_grid->x_min + 1, the_grid->y_max - the_grid->y_min + 1);

    /* Now print each element of the grid. */

    for (i = the_grid->x_min; i <= the_grid->x_max; i++) {
      for (j = the_grid->y_min; j <= the_grid->y_max; j++) {
        if ((solution_only == False) && (the_grid->grid[i][j].letter == EMPTY_GRID_CELL)) {
          fprintf( outfile, "%c", 'a'+(rand() % 26) );
        } else {
          fprintf( outfile, "%c", the_grid->grid[i][j].letter );
        }
      }
      fprintf (outfile, "\n");
    }
  }
}

/* Given a grid, adjust the x_min, x_max, y_min, and y_max fields to represent
   the portion of the grid that has letters filled in from the word search
   puzzle.  Shrinking the dimensions means that printing the grid only
   prints the part that we're using, so each of the outer boundaries have at
   least one letter in them used in a word of the puzzle. 

*/

void
grid_shrink( grid *the_grid ) {
  int i, j;
  int count;

  if (the_grid != NULL) {

    /* Get rid of the left columns that have no characters in them. 
       The inner loop counts how many cells in a row/column are in use.
       The outer loop then moves the outer boundary of the grid for as long
       as that inner loop reports no cells being used.  Because we exit the 
       outer loop by incrementing the loop variable, we adjust it by 1 on the
       way out to account for the columnt that we found holding data right
       before we incremented the loop counter.
    */

    count = 0;
    for (the_grid->x_min = 0; (count == 0) && (the_grid->x_min < the_grid->cols); the_grid->x_min++) {
      for (j = 0; j < the_grid->cols; j++) {
        count += (the_grid->grid[the_grid->x_min][j].letter == EMPTY_GRID_CELL ? 0 : 1 );
      }
    }
    the_grid->x_min--;

    /* Get rid of the right columns that have no characters in them. */
  
    count = 0;
    for (the_grid->x_max = the_grid->cols-1; (count == 0) && (the_grid->x_max >= 0); the_grid->x_max--) {
      for (j = 0; j < the_grid->rows; j++) {
        count += (the_grid->grid[the_grid->x_max][j].letter == EMPTY_GRID_CELL ? 0 : 1 );
      }
    }
    the_grid->x_max++;

    /* Get rid of the lower rows that have no characters. */
  
    count = 0;
    for (the_grid->y_min = 0; (count == 0) && (the_grid->y_min < the_grid->rows); the_grid->y_min++) {
        for (i = the_grid->x_min; i <= the_grid->x_max; i++) {
        count += (the_grid->grid[i][the_grid->y_min].letter == EMPTY_GRID_CELL ? 0 : 1 );
      }
    }
    the_grid->y_min--;

    /* Get rid of the upper rows that have no characters. */
  
    count = 0;
    for (the_grid->y_max = the_grid->rows-1; (count == 0) && (the_grid->y_max >= 0); the_grid->y_max--) {
      for (i = the_grid->x_min; i <= the_grid->x_max; i++) {
        count += (the_grid->grid[i][the_grid->y_max].letter == EMPTY_GRID_CELL ? 0 : 1 );
      }
    }
    the_grid->y_max++;
  }
}

/* Given a word, a grid, an (x,y) coordinate in the grid and a vector for the 
   word to flow in (xdir, ydir), report back on whether or not the word
   can fit into the grid.  To fit, the word must be within the bounds of the 
   grid and any letters currently in the grid must match the letters of
   the word at the corresponding location.

   Return True if the the word could be placed into the grid and False if it
   cannot.

   When the word fits, we also return, as a parameter, how many letters of the
   word are matching letters that are already in the grid at the given
   position.

*/

boolean
grid_wordfits( grid *the_grid, char *word, int x, int y, int xdir, int ydir, int *count ) {
  boolean fit = False;
  int len;
  int i;

  if ((the_grid != NULL) && (word != NULL)) {
    len = strlen( word );

    /* See if the word fits within the bounds of the grid itself. */

    if ( ((x + len*xdir) >= 0) && ((x + len*xdir) < the_grid->cols) &&
         ((y + len*ydir) >= 0) && ((y + len*ydir) < the_grid->rows) ) {

      /* There is room in the grid for the word.  See if it conflicts
         with any letters already in the grid.  The "count" variable
         keeps track of the number of matching letters between the
         word and the grid. */

      *count = 0;
      fit = True;
      for (i = 0; (fit == True) && (i < len); i++) {
        if ( word[i] == the_grid->grid[x + i*xdir][y + i*ydir].letter) {
          /* Letters agree. */
          *count++;
        } else if (the_grid->grid[x + i*xdir][y + i*ydir].letter == EMPTY_GRID_CELL) {
          /* An empty cell.  All ok to make it go. */

          /* Try to lock the empty cell, if the cell is already locked, discard this try */
          if (sem_trywait(&(the_grid->grid[x + i*xdir][y + i*ydir].cell_lock)) == EAGAIN){
            fit = False;

            /* If it is already locked, unlock all cells before it in this try */
            unlock_cells(the_grid, x, y, xdir, ydir, i);

            /*for (n=0; n<i; n++){
              int success=sem_getvalue(&(the_grid->grid[x + n*xdir][y + n*ydir].cell_lock), &lock);
              if (success == 0 && lock <= 0)
                sem_post(&(the_grid->grid[x + n*xdir][y + n*ydir].cell_lock));
              else if (success == -1){
                perror(strerror(errno));
              }
            }*/
          }
        } else {
          /* A conflict in letters.  Abandon this spot. */  

          /* Unlock every cell behind it */
          unlock_cells(the_grid, x, y, xdir, ydir, i);

          /*for (n=0; n<i; n++){
            int success=sem_getvalue(&(the_grid->grid[x + n*xdir][y + n*ydir].cell_lock), &lock);
            if (success == 0 && lock <= 0)
              sem_post(&(the_grid->grid[x + n*xdir][y + n*ydir].cell_lock));
            else if (success == -1){
              perror(strerror(errno));
            }
          }*/
            fit = False;
        }
      }
    }
  }

  return fit;
}

/* Put a word in the grid at the given (x,y) position and going in 
   direction (xdir, ydir).  The function assumes that the word does not
   conflict with what is currently in the grid.  i.e. a call to
   grid_wordfits has returned True for the grid, word, and position
   combination. */

void
grid_placeword( grid *the_grid, char *word, int x, int y, int xdir, int ydir ) {
  int i;
  int len;

  if ((the_grid != NULL) && (word != NULL)) {

    /* Copy over the word character by character. */

    len = strlen( word );
    for (i = 0; i < len; i++) {
      the_grid->grid[x + i*xdir][y + i*ydir].letter = word[i];
    }
  }
}

