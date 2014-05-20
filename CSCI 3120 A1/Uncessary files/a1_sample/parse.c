
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define BUF_MAX  (81)
#define NAME_MAX (21)

typedef struct {
  char name[NAME_MAX];
  int  lifetime;
} pcb_t;

int
main( int argc, char **argv )
{
  int    error_code = 0;  /* In unix, a program returns 0 if all is ok. */
  List_t processes;
  char   inputbuffer[BUF_MAX];
  char   name[BUF_MAX];
  int    lifetime;
  pcb_t  *pcb;

  /* Initialize my data structures. */

  if (List_init( &processes )) {

    /* This creates an infinite loop from which you will need to do a
       ^C to stop the program.  This may not be the most elegant solution
       for you. */

    while (1) {
      if (fgets( inputbuffer, BUF_MAX-1, stdin )) {
	/* Put the parameters into a PCB and store it in the list of processes. */
	if (sscanf( inputbuffer, "%s %d", name, &lifetime) == 2) {
	  /* We have all the input that is required. */

	  pcb = (pcb_t *) malloc( sizeof( pcb_t ) );
	  if (pcb != NULL) {
	    strncpy( pcb->name, name, NAME_MAX-1 );
	    pcb->name[NAME_MAX] = '\0'; /* Make sure that it is null-terminated. */
	    pcb->lifetime = lifetime;
	    printf ("Read and stored process %s with lifetime %d\n", pcb->name, pcb->lifetime);

	    if (List_add_tail( &processes, (void *)pcb ) == 0) {
	      printf ("Error in inserting the process into the list.\n");
	    }
	  } else {
	    printf( "Unable to allocate enough memory.\n");
	  }
	} else {
	  printf ("Incorrect order of parameters read.\n");
	}
      } else {
	printf ("Nothing read in.\n");
      }
    }

  } else {
    printf ("Unable to initialize the list of processes.\n");
    error_code = 1;
  }

  return error_code;
}

