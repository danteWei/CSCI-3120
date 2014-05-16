
#include <stdlib.h>
#include <stdio.h>

#define _DONT_SUBSTITUTE
#include "my_memory.h"

static int space_used = 0;
static int space_allocated = 0;

void *get_memory( int size )
{
  int *a = NULL;
  void *p = NULL;

  printf ("my malloc\n");
  space_used += size + sizeof( int );
  space_allocated += size + sizeof( int );
  a =  malloc( size + sizeof( int ) );
  if (a != NULL) {
    a[0] = size;
    p = (void *) &(a[1]);
  }

  return p;
}

void release_memory( void *p )
{
  int *a = (int *)p;

  printf ("my free\n");

  if (p != NULL) {
    space_allocated -= a[-1] + sizeof(int);
  }

  free( (void *)&(a[-1]) );
}

void *grow_memory( void *p, int new_size )
{
  int *a, *b;
  void *x = NULL;
  int old_size;

  printf ("my realloc\n");
  b = (int *) p;
  old_size = b[-1];
  a = realloc( (void *)&(b[-1]), new_size + sizeof( int ) );
  if (a != NULL) {
    a[0] = new_size;
    x = (void *)&(a[1]);
    space_used += new_size;
    space_allocated += new_size - old_size;
  }

  return x;
}

int init_memory( int size )
{
  printf ("memory initialized to size %d\n", size);
  return 1;
}

void end_memory( void )
{
  printf ("memory closed down.  Used %d bytes and end with %d allocated bytes\n", space_used, space_allocated);
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

