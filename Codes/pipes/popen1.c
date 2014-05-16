
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define LINELEN (80)

int
main( int argc, char **argv )
{
  int return_value = 0;
  char line[LINELEN+1];
  FILE *pipe_from_shell;

  /* Execute the "ls" command in a seprate process. */

  pipe_from_shell = popen ("ls", "r");
  if (pipe_from_shell) {
    while (fgets(line, LINELEN, pipe_from_shell) > 0) {
      printf ("Shell said: %s", line );
    }

    pclose( pipe_from_shell );
  }

  return return_value;
}

