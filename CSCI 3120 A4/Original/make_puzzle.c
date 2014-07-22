#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "wordlist.h"
#include "grid.h"

#define MAX_TRIES (1000)
#define GROW_RATE (100)

typedef struct {
  unsigned int seed;
  char *outfilename;
  boolean solution_only;
} config_info;

void
get_configuration( int argc, char **argv, config_info *config ) {
  int i;

  if (config != NULL) {
    /* Define default parameter values. */

    config->seed = 10;
    config->outfilename = NULL;
    config->solution_only = False;

    /* See if any parameter values are now overridden. */

    i = 1;
    while (i < argc) {
      if ((argv[i][0] == '-') && (strlen( argv[i] ) > 1)) {
        /* We have a configuration parameter. */
        switch( argv[i][1] ) {
          case 's':
            /* New random seed value. */

            if (i < argc) {
              i++;
              config->seed = atoi( argv[i] );
            }

            break;
          case 'o':
            /* Send the data to a file rather than stdout. */

            if (i < argc) {
              i++;
              config->outfilename = argv[i];
            }

            break;
          case 'g':
            /* Only print the solution grid. */

            config->solution_only = True;

            break;
          default:
            /* Just ignore the parameter. */
            break;
        }
      }
        i++;
    }
  }
}
void
place_words( wordlist *the_words, grid *the_grid, unsigned int seed ) {

  int num_words;
  int to_place;
  char *word;
  int placed;
  int x, y, xdir, ydir;
  int x_opt, y_opt, xdir_opt, ydir_opt;
  boolean fits;
  int most_matches;
  int scope; /* How big of a search space do we consider. */
  int feasible_fits;
  int attempts;
  int count;
  int rows, cols;

  if ((the_words != NULL) && (the_grid != NULL)) {

    num_words = wordlist_numwords( the_words );
    grid_getdimensions( the_grid, &rows, &cols );
 
    for (placed = 0; placed < num_words; placed++) {
      /* Find an unplaced word to tackle. */
    
      to_place = rand_r( &seed ) % num_words;
      word = wordlist_getword( the_words, to_place );
      if (word != NULL) {
  
        /* We now know which word to place.  Find a spot for it. 
           Begin by assuming that we have no placement that works 
           (most_matches < 0) and we begin searching for a placement 
           within a grid scope barely larger than the word itself. 
 
           The strategy is twofold.  First, we look for a possible spot
           close to the centre of the grid and grow the set of possible
           placements from there.  This strategy aims to concentrate all 
           the words into one spot to make a small grid.

           Second, once we find a placement, we aren't happy with it.
           We remember that spot and the look for something at least as
           good.  Only after considering MAX_TRIES competitive / improved
           placements that we finally choose our best spot to place the word.
        */
    
        most_matches = -1;
        scope = strlen(word)/2 + 1;
   
        attempts = 0;
        feasible_fits = 0; 

        while (feasible_fits < MAX_TRIES) {
          attempts++;
          /* Every now and then, increase the size of the grid for which 
             we'll look for a placement of the word. */
  
          if ((scope < the_grid->rows) && ((attempts % GROW_RATE) == 0)) {
            scope++;
          }
      
          /* Get a position for the new item. */
     
          x = rand_r( &seed ) % scope - (scope/2) + cols/2;
          y = rand_r( &seed ) % scope - (scope/2) + rows/2;
  
          /* Pick a direction for the line.  At least one of the x and y 
             directions must be non-zero. */
  
          xdir = 0; ydir = 0;
          while ((xdir == 0) && (ydir == 0)) {
            xdir = rand_r( &seed ) % 3 - 1;
            ydir = rand_r( &seed ) % 3 - 1;
          }
      
          /* See if the word fits in. */
     
          fits = grid_wordfits( the_grid, word, x, y, xdir, ydir, &count );
 
          if ((fits == True) && (count <= most_matches)) {
            feasible_fits++;
          }
 
          /* Use this new placement if there are more matching spots or if
             there is the same number of matching spots but the placement is
             closer to the centre of the grid, trying to keep the grid small. */
  
          if ((fits == True) && ((count > most_matches) || ((count == most_matches) && (abs(x-cols/2) + abs(y-rows/2) < abs(x_opt-cols/2) + abs (y_opt-rows/2))))) {
            /* Remember this better placement than what we had. */
  
            most_matches = count;
            x_opt = x;
            y_opt = y;
            xdir_opt = xdir;
            ydir_opt = ydir;
          }
        }
    
        if (most_matches >= 0) {
          /* Put in the word. */
  
          grid_placeword( the_grid, word, x_opt, y_opt, xdir_opt, ydir_opt ); 
        } else {
          printf ("Couldn't place word %s\n", word);
        }
      }
    }
  }
}



int
main( int argc, char **argv ) {
  int return_code = 0;
  wordlist the_words;
  boolean can_proceed;
  int character_count;
  grid the_grid;
  config_info config;
  FILE *outfile;

  get_configuration( argc, argv, &config );

  if (wordlist_init( &the_words ) == True) {
 
    /* Get a seed for the random number generator so that we can repoduce the results. */

    /* Read in the words to place in the puzzle. */

    can_proceed = wordlist_load( &the_words, stdin );
    if (can_proceed == True) {

      /* Initialize the grid. */

      character_count = wordlist_totalchars( &the_words );
      if (grid_init( &the_grid, character_count ) == True) {

        place_words( &the_words, &the_grid, config.seed );

        /* Print the resulting grid. */
   
        grid_shrink( &the_grid ); 

        if (config.outfilename == NULL) {
          outfile = stdout;
        } else {
          outfile = fopen( config.outfilename, "w" );
          if (!outfile) {
            printf ("Unable to open output file.\n");
          }
        }
        if (outfile != NULL) {
          grid_print( &the_grid, outfile, config.solution_only ); 
          if (config.outfilename != NULL) {
            fclose( outfile );
          }
        }
  
        /* Clean up as I leave. */
  
        grid_free( &the_grid );

      } else {
        printf ("Unable to create the grid.\n");
      }
    } else {
      printf ("Unable to initialize the word list.\n");
    }
    wordlist_free( &the_words );
  }

  return return_code;
}

