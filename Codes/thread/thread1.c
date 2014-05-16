
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

void *
count_up( void *parameter )
{
  int i;
  int limit = (int)parameter;

  for (i = 0; i < limit; i++) {
    printf ("%d\n", i );
  }

  return NULL;
}

int
main( int argc, char **argv )
{
  pthread_t proc1, proc2;
  pthread_attr_t attrib;                                                  

  /* Allocate a LWP for each thread so they run concurrently */  

  pthread_attr_init( &attrib );                                           
  pthread_attr_setscope( &attrib, PTHREAD_SCOPE_SYSTEM );                 

  /* Create the lightweight processes */

  pthread_create( &proc1, &attrib, count_up, (void *)2000 );
  pthread_create( &proc2, &attrib, count_up, (void *)1000 );

  /* Wait for both lightweight processes to end. */

  pthread_join( proc1, NULL );
  pthread_join( proc2, NULL );

  return 0;
}



