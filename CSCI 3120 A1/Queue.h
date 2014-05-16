//Header file for Queue
//Author: Xinjing Wei

#ifndef _Queue_h_
#define _Queue_h_

#include "List.h"

//Define Queue structure
typedef struct {
	char *name;
	List_t *list;
	List_node_t *head;
} Queue;

int Queue_init(Queue *q, char *qName);
char *getQueueName(Queue *q);
void Queue_clear(Queue *q);
int Queue_insert(void *data, Queue *q);
int Queue_add_head(void *data, Queue *q);
int Queue_add_tail(void *data, Queue *q);
int Queue_head_info(Queue *q, void **data);
int Queue_remove_first(Queue *q, void **data);
int size(Queue *q);
List_node_t *Queue_next(Queue *q, List_node_t **curr);


#endif
