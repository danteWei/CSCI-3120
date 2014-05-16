
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define LINELEN (80)
#define PIPE_READ_END (0)
#define PIPE_WRITE_END (1)

int
main( int argc, char **argv )
{
  int return_value = 0;
  pid_t process_id;
  int retval;
  int pipe_ids[2];
  char line[LINELEN+1];

  /* Set up pipes over which the processes can communicate.  Recall that
     pipes are unidirectional. 

     pipe_id[0] is the reading end of the pipe.
     pipe_id[1] is the writing end of the pipe.  */

  if (pipe( pipe_ids ) == 0) {
    /* Now we can get two processes to communicate. */

    process_id = fork();
    if (process_id == 0) {
      /* In the child. I only use */

      close( pipe_ids[PIPE_WRITE_END] );

      /* Wait for information from the parent and then print it. */

      while (read(pipe_ids[PIPE_READ_END], line, LINELEN) > 0) {
        line[LINELEN] = '\0';
        printf ("Child received: %s\n", line );
      }

      /* No more data. so clean up and exit. */

      close( pipe_ids[PIPE_READ_END] );

    } else {
      /* In the parent.  Send some data to the child and then end. */

      close( pipe_ids[PIPE_READ_END] );

      fgets( line, LINELEN, stdin );
      line[LINELEN] = '\0';

      write( pipe_ids[PIPE_WRITE_END], line, strlen(line) );

      close( pipe_ids[PIPE_WRITE_END] );

      wait( NULL );
    }
  }

  return return_value;
}

