
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

typedef struct {
  int start;
  int limit;
} param_list;

void *
count_up( void *parameter )
{
  int i;
  param_list *info = (param_list *)parameter;

  for (i = info->start; i < info->start + info->limit; i++) {
    printf ("%d\n", i );
  }

  return NULL;
}

int
main( int argc, char **argv )
{
  param_list info1, info2;

  pthread_t proc1, proc2;
  pthread_attr_t attrib;                                                  

  /* Allocate a LWP for each thread so they run concurrently */  

  pthread_attr_init( &attrib );                                           
  pthread_attr_setscope( &attrib, PTHREAD_SCOPE_SYSTEM );                 

  /* Create the lightweight processes */

  info1.start = 10;
  info1.limit = 20;

  pthread_create( &proc1, &attrib, count_up, (void *)&info1 );

  info2.start = 10000;
  info2.limit = 10;

  pthread_create( &proc2, &attrib, count_up, (void *)&info2 );

  /* Wait for both lightweight processes to end. */

  pthread_join( proc1, NULL );
  pthread_join( proc2, NULL );

  return 0;
}



