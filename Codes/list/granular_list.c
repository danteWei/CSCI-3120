
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LIMIT 10000

typedef struct _node_ {
  int value;
  sem_t lock;
  struct _node_ *next;
} node;

node head;

void
add_to_list( int value )
{
  node *new_node, *runner;

  /* Create a new node with the new value in it. */

  new_node = (node *) malloc( sizeof( node ) );
  new_node->value = value;
  new_node->next = NULL;
  sem_init( &new_node->lock, 0, 1 );

  /* Find the end of the linked list */

  runner = &head;
  sem_wait( &runner->lock );
  while (runner->next != NULL) {
    /* Move my lock. */

    if (runner->next != NULL) {
      sem_wait( &runner->next->lock );
      sem_post( &runner->lock );
    }

    runner = runner->next;
  }

  /* Append the new node to the linked list. */

  runner->next = new_node;

  sem_post( &runner->lock );
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
  sem_init( &head.lock, 0, 1 );

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

  sem_wait( &head.lock );

  num_nodes = 0;
  runner = head.next;
  while (runner != NULL) {
    num_nodes = num_nodes + 1;
    runner = runner->next;
  }

  sem_post( &head.lock );

  printf ("There are %d nodes in the list\n", num_nodes);

  return 0;
}



