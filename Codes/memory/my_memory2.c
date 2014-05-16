
#include <stdlib.h>
#include <stdio.h>

#define _DONT_SUBSTITUTE
#include "my_memory.h"

static int space_used = 0;

void *get_memory( int size )
{
  printf ("my malloc\n");
  space_used += size;
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
  space_used += new_size;
  return realloc( p, new_size );
}

int init_memory( int size )
{
  printf ("memory initialized to size %d\n", size);
  return 1;
}

void end_memory( void )
{
  printf ("memory closed down.  Allocated %d bytes\n", space_used);
}








void *get_memory_track( int size, char *file, int line )
{
  printf ("my malloc called at %s:%d\n", file, line);
  space_used += size;
  return malloc( size );
}

void *grow_memory_track( void *p, int new_size, char *file, int line )
{
  printf ("my realloc called at %s:%d\n", file, line);
  space_used += new_size;
  return realloc( p, new_size );
}

