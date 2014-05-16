
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LIMIT 10000

typedef struct _node_ {
  int value;
  struct _node_ *next;
} node;

node head;
node *tail = &head;

void
add_to_list( int value )
{
  node *new_node, *runner;

  /* Create a new node with the new value in it. */

  new_node = (node *) malloc( sizeof( node ) );
  new_node->value = value;
  new_node->next = NULL;

  /* Append the new node to the linked list. */

  tail->next = new_node;
  tail = new_node;
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

  /* Allocate a LWP for each thread so they run concurrently */  

  pthread_attr_init( &attrib );                                           
  pthread_attr_setscope( &attrib, PTHREAD_SCOPE_SYSTEM );                 

  /* Create the lightweight processes */

  pthread_create( &proc1, &attrib, insert_loop, NULL );
  pthread_create( &proc2, &attrib, insert_loop, NULL );

  /* Wait for both lightweight processes to end. */

  pthread_join( proc1, NULL );
  pthread_join( proc2, NULL );

  /* Count up the number of nodes in the list and report it. */

  num_nodes = 0;
  runner = head.next;
  while (runner != NULL) {
    num_nodes = num_nodes + 1;
    runner = runner->next;
  }

  printf ("There are %d nodes in the list\n", num_nodes);

  return 0;
}



