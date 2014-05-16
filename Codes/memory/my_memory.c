
#include <stdlib.h>
#include <stdio.h>

#define _DONT_SUBSTITUTE
#include "my_memory.h"

void *get_memory( int size )
{
  printf ("my malloc\n");
  return malloc( size );
}

void release_memory( void *p )
{
  printf ("my free\n");
  free( p );
}

void *grow_memory( void *p, int new_size )
{
  printf ("my realloc\n");
  return realloc( p, new_size );
}

int init_memory( int size )
{
  printf ("memory initialized to size %d\n", size);
  return 1;
}

void end_memory( void )
{
  printf ("memory closed down\n");
}








void *get_memory_track( int size, char *file, int line )
{
  printf ("my malloc called at %s:%d\n", file, line);
  return malloc( size );
}

void *grow_memory_track( void *p, int new_size, char *file, int line )
{
  printf ("my realloc called at %s:%d\n", file, line);
  return realloc( p, new_size );
}

