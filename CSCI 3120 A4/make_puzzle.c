#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "common.h"
#include "wordlist.h"
#include "grid.h"

#define MAX_TRIES (1000)
#define GROW_RATE (100)
#define MAX_THREADS (40) // Max number of threads a user can create

typedef struct {
  unsigned int seed;
  char *outfilename;

  int numThreads; //stores the number of threads a user wants to create

  boolean solution_only;
  boolean multi_thread;
} config_info;

typedef struct {
  int num_words_left;
  sem_t lock;
} num_words_left;

/* A package of information needed for placing words */
typedef struct {
  wordlist *the_words;
  grid *the_grid;
  unsigned int seed;
  num_words_left *numwordleft;
} thread_package;


num_words_left numwordleft;

/* Initialize the package */
boolean thread_pack_init(thread_package *pack, wordlist *words, grid *grid, unsigned int seed, num_words_left *n){
  boolean init = False;

  if (pack != NULL){
    pack->the_words=words;
    pack->the_grid=grid;
    pack->seed=seed;
    pack->numwordleft=n;

    init = True;
  }

  return init;
}

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
          case 't':
            /* create n threads */
            config->multi_thread = True;
            if (i < argc) {
              i++;

              /* Limits the number of threads within 40 */

              if (atoi( argv[i] ) <= MAX_THREADS && atoi( argv[i] ) != 0)
                config->numThreads = atoi( argv[i] );
              else if (atoi( argv[i] ) == 0){
                printf("The number of threads you want to create is either not specified or not a positive integer\n Program will use 1 thread.\n");
                config->numThreads = 1;
              }
              else {
                printf("The number of threads you want to create exceeds the limit (%d threads).\n Program will take %d as the number followed by \"-t\".\n", MAX_THREADS, MAX_THREADS);
                config->numThreads = MAX_THREADS;
              }
            }
            else{
              config->numThreads = 1;
              printf("Number of threads is set to 1\n");
            }

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
*place_words( void *pack ) {

  thread_package *thread_pack = (thread_package *) pack;
  wordlist *the_words = thread_pack->the_words;
  grid *the_grid = thread_pack->the_grid;
  unsigned int seed = thread_pack->seed;
  num_words_left *numwordleft = thread_pack->numwordleft;

  int num_words;
  int to_place;
  char *word;
  int unplaced;
  int x, y, xdir, ydir;
  int x_opt, y_opt, xdir_opt, ydir_opt;
  boolean fits;
  int most_matches;
  int scope; /* How big of a search space do we consider. */
  int feasible_fits;
  int attempts;
  int count;
  int rows, cols;
  int placed=0;

  if ((the_words != NULL) && (the_grid != NULL)) {

    num_words = wordlist_numwords( the_words );
    grid_getdimensions( the_grid, &rows, &cols );
    
    sem_wait(&(numwordleft->lock));
    unplaced = numwordleft->num_words_left;
    sem_post(&(numwordleft->lock));


    while(unplaced > 0) {
      /* Find an unplaced word to tackle. */
    
      boolean first_fit=True; //determine if a solution is the first solution or not
      to_place = rand_r( &seed ) % num_words;
      word = wordlist_getword( the_words, to_place );
      //printf("the word is: %s\n", word);

      if (word != NULL && unplaced != 0) {
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
        
        sem_wait(&(numwordleft->lock));
        int *tmp = &(numwordleft->num_words_left);
        if (*tmp > 0)
          *tmp = *tmp - 1;
        
        unplaced = *tmp;
        sem_post(&(numwordleft->lock));


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

            /* dicard this solution */
            if (count < most_matches)
              unlock_cells(the_grid, x, y, xdir, ydir, strlen(word));
            if ((count == most_matches) && ((abs(x-cols/2) + abs(y-rows/2) >= abs(x_opt-cols/2) + abs (y_opt-rows/2))))
              unlock_cells(the_grid, x, y, xdir, ydir, strlen(word));
          }
 
          /* Use this new placement if there are more matching spots or if
             there is the same number of matching spots but the placement is
             closer to the centre of the grid, trying to keep the grid small. */
  
          if ((fits == True) && ((count > most_matches) || ((count == most_matches) && (abs(x-cols/2) + abs(y-rows/2) < abs(x_opt-cols/2) + abs (y_opt-rows/2))))) {
            /* Remember this better placement than what we had. */
            if (first_fit == True)
              first_fit = False;

            /* If this is not the first solution, unlock the previous optimal fits */

            else
              unlock_cells(the_grid, x_opt, y_opt, xdir_opt, ydir_opt, strlen(word));
            
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
          placed++;
          /* After puting in the word, uncock those cells */
          unlock_cells(the_grid, x_opt, y_opt, xdir_opt, ydir_opt, strlen(word));
        } else {
          printf ("Couldn't place word %s\n", word);
        }
        sem_wait(&(numwordleft->lock));
        tmp = &(numwordleft->num_words_left);;
        unplaced = *tmp;
        sem_post(&(numwordleft->lock));
      } /* End of if (word != NULL) */
      
    } /* End of for loop */
  }
  printf("%d words have been placed\n", placed);
  return NULL;
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
  

  config.multi_thread = False;

  get_configuration( argc, argv, &config );

  if (wordlist_init( &the_words ) == True) {
 
    /* Get a seed for the random number generator so that we can repoduce the results. */

    /* Read in the words to place in the puzzle. */

    can_proceed = wordlist_load( &the_words, stdin );
    if (can_proceed == True) {

      /* Initialize the grid. */

      character_count = wordlist_totalchars( &the_words );
      if (grid_init( &the_grid, character_count ) == True) {

        numwordleft.num_words_left=wordlist_numwords(&the_words);

        if (sem_init(&(numwordleft.lock), 0, 1) == -1)
          perror(strerror(errno));

        thread_package thread_pack;

        if (thread_pack_init( &thread_pack, &the_words, &the_grid, config.seed, &numwordleft) != True ){
          printf("Unable to Initialize thread package.\n");
          exit(EXIT_FAILURE);
        }

        int numThreads = 1; //default thread number

        /* Initiate threads */
        if (config.multi_thread == True)
          numThreads=config.numThreads;
        if (numThreads <= 0){
          printf("The minimum thread number should always be positive.\n");
          numThreads=1;
        }

        pthread_attr_t attrib; // thread attributes
  
        pthread_attr_init(&attrib);
        pthread_attr_setscope(&attrib, PTHREAD_SCOPE_SYSTEM);

        /* Commented out due to weird bug */
        /*pthread_t *thread_list=(pthread_t *) malloc(sizeof(pthread_t));
        if (thread_list == NULL){
          printf("System out of memory\n");
          exit(EXIT_FAILURE);
        }*/
        pthread_t thread_list[MAX_THREADS+1];

        int n;
        for (n=0; n<numThreads; n++){
          pthread_t p;
          pthread_create(&p, &attrib, place_words, (void *)&thread_pack);
          thread_list[n]=p;
        }
        /* Wait for threads to finish */

        for (n=0; n<numThreads; n++){
          pthread_join(thread_list[n], NULL);
          printf("Thread %d terminated.\n", n);
        }
        /*
        if (thread_list != NULL)
          free(thread_list);
        */
        //place_words( (void *)&thread_pack );

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

