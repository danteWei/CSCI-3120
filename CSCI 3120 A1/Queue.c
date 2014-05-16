//A class for creating a queue
//Author: Xinjing Wei
//Coding style learned from McAllister's codes

#include<stdlib.h>
#include "Queue.h"
#include "List.h"

//Initialize a queue
int Queue_init(Queue *q, char *qName){
	int all_ok=0;
	q->name=qName;
	//Set the queue info to NULL pointers initially
	if(q != NULL){
		List_t *nlist;
		List_init(nlist);
		q->list=nlist;
		q->head=(q->list)->head;
		all_ok=1;
	}

	return all_ok;
}

//Get the name of the queue
char *getQueueName(Queue *q){
	return q->name;
}

//delete the queue and release all related memories
void Queue_clear(Queue *q){
	//Free the list in the queue
	List_destroy(q->list);

	//Change the head pointer to NULL
	q->head=NULL;
}

//Add data to the front of the queue
int Queu_add_head(void *data, Queue *q){
	int all_ok=List_add_head(q->list, data);

	//update the head pointer
	q->head=q->list->head;
	return all_ok;
}

//Add data to the end of the queue
int Queue_add_tail(void *data, Queue *q){
	return List_add_tail(data, q->list);
}

//Insert an item to a specific postion in the queue
int Queue_insert(void *data, Queue *q){
	int all_ok=0;

	return all_ok;
}


//return the information of the head item
int Queue_head_info(Queue *q, void **data){
	return List_head_info(q->list, data);
}

//remove and return the information of the head item in the queue
int Queue_remove_first(Queue *q, void **data){

	//update the head pointer
	q->head=q->head->next;
	return List_remove_head(q->list, data);
}

//traverse through the queue
List_node_t *Queue_next(Queue *q, List_node_t **curr){
	List_node_t **next=(List_node_t **)curr;

	if(*next == NULL){
		*next=q->head;
	}
	else{
		*next=(*curr)->next;
	}
	return *next;
}

//return the size of the queue
int size(Queue *q){

	//If the queue is not initialized, return -1
	if(q == NULL){
		return -1;
	}

	//If the queue is empty, return 0
	else if(q->list == NULL){
		return 0;
	}
	else{
		int count=0;
		List_node_t *curr=q->head;
		while(curr != NULL){
			count++;
			curr=curr->next;
		}
		return count;
	}
}

