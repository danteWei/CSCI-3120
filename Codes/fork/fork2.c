
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int
main( int argc, char **argv )
{
  int return_value = 0;
  pid_t process_id;
  int retval;
  char *path[] = { "/Users", NULL };

  process_id = fork();
  if (process_id == 0) {
    /* In the child. */
    printf ("I am the child!\n");

    /* Run another program and then end. */

    execv( "/bin/ls", path );
  } else {
    /* In the parent.  Wait for the child to end. */
    printf ("I am the parent!\n");

    waitpid( process_id, &retval, 0);

    printf ("Child has ended.\n");
  }

  return return_value;
}

