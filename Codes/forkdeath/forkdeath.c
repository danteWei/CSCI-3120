
#include <stdlib.h>

int
main( int argc, char **argv )
{
  int return_code = 0;
  int i;

  /* Well....almost fork death.  Build in a termination condition. */

  for (i = 0; i < 10000; i++) {
    fork();
  }

  return return_code;
}

