
#include <stdlib.h>
#include <stdio.h>

#define _DONT_SUBSTITUTE
#include "my_memory.h"

static int space_used = 0;
static int space_allocated = 0;

#define CANARY_OK = 0xDEADBEEF

typedef struct {
  int size;
  long canary;
} mem_header_t;

void *get_memory( int size )
{
  mem_header_t *a = NULL;
  void *p = NULL;

  printf ("my malloc\n");
  space_used += size + sizeof( mem_header_t );
  space_allocated += size + sizeof( mem_header_t );
  a =  malloc( size + sizeof( mem_header_t ) );
  if (a != NULL) {
    a[0].size = size;
    a[0].canary = (long)CANARY_OK;
    p = (void *) &(a[1]);
  }

  return p;
}

void release_memory( void *p )
{
  mem_header_t *a = (mem_header_t *)p;

  printf ("my free\n");

  if (p != NULL) {
    space_allocated -= a[-1].size + sizeof(mem_header_t);
    a[-1].canary = 0;
  }

  free( (void *)&(a[-1]) );
}

void *grow_memory( void *p, int new_size )
{
  mem_header_t *a, *b;
  void *x = NULL;
  int old_size;

  printf ("my realloc\n");
  b = (mem_header_t *) p;
  old_size = b[-1].size;
  a = realloc( (void *)&(b[-1]), new_size + sizeof( mem_header_t ) );
  if (a != NULL) {
    a[0].size = new_size;
    a[0].canary = (long)CANARY_OK;
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

