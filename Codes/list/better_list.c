
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LIMIT 10000

typedef struct _node_ {
  int value;
  struct _node_ *next;
} node;

node head;
sem_t lock;

void
add_to_list( int value )
{
  node *new_node, *runner;

  /* Create a new node with the new value in it. */

  new_node = (node *) malloc( sizeof( node ) );
  new_node->value = value;
  new_node->next = NULL;

  /* Find the end of the linked list */

  sem_wait( &lock );

  runner = &head;
  while (runner->next != NULL) {
    runner = runner->next;
  }

  /* Append the new node to the linked list. */

  runner->next = new_node;

  sem_post( &lock );
}

void *
insert_loop( void *empty_parameter )
{
  int i;

  for (i = 0; i < LIMIT; i++) {
    add_to_list( i );
  }

  return NULL;
}

int
main( int argc, char **argv )
{
  pthread_t proc1, proc2;
  pthread_attr_t attrib;                                                  
  int       num_nodes;
  node      *runner;

  head.next = NULL;

  /* Initialize my semaphore as unlocked */

  if (sem_init( &lock, 0, 1 ) != 0) {
    printf ("Unable to create the semaphore.  Aborting.\n");
  } else {

    /* on Solaris, allocate a LWP for each thread so they run concurrently */  

    pthread_attr_init( &attrib );                                           
    pthread_attr_setscope( &attrib, PTHREAD_SCOPE_SYSTEM );                 

    /* Create the lightweight processes */

    pthread_create( &proc1, &attrib, insert_loop, NULL );
    pthread_create( &proc2, &attrib, insert_loop, NULL );

    /* Wait for both lightweight processes to end. */

    pthread_join( proc1, NULL );
    pthread_join( proc2, NULL );


    /* Count up the number of nodes in the list and report it. */

    sem_wait( &lock );

    num_nodes = 0;
    runner = head.next;
    while (runner != NULL) {
      num_nodes = num_nodes + 1;
    runner = runner->next;
    }

    sem_post( &lock );

    printf ("There are %d nodes in the list\n", num_nodes);
  }

  return 0;
}



