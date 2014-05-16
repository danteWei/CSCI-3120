
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
  char *to_sort[] = {
     "one"
    ,"two"
    ,"three"
    ,"four"
  };
  char *filename = NULL;
  FILE *file_pointer = NULL;
  int i;
  int entries;
  char command[LINELEN+1];

  /* Put the strings into a file. */

  entries = sizeof(to_sort) / sizeof(to_sort[0]);
  filename = tmpnam( NULL );
  if (filename) {
    file_pointer = fopen( filename, "w" );
    if (file_pointer) {
      for (i = 0; i < entries; i++) {
        fprintf( file_pointer, "%s\n", to_sort[i] );
      }
      fclose( file_pointer );
    }

    /* Execute the "sort" command in a seprate process. */

    sprintf( command, "sort %s", filename );

    pipe_from_shell = popen (command, "r");
    if (pipe_from_shell) {
      while (fgets(line, LINELEN, pipe_from_shell) > 0) {
        printf ("Shell said: %s", line );
      }
  
      pclose( pipe_from_shell );
    }

    /* Get rid of the file now that it's done. */

    unlink( filename );
  }

  return return_value;
}

