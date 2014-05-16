
#include <stdlib.h>
#include "my_memory.h"

int
main (int argc, char **argv) {
  int return_value = 0;

  void *mem1, *mem2;

  mem1 = malloc( 50 * sizeof(int) );
  mem2 = realloc( mem1, 60*sizeof( int ) );
  free( mem2 );

  return return_value;
}

