#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>	//for print error messages
#include "common.h"

/* define constants */
#define MAXTHREAD (10)
#define MAXLEN (80)
#define MINMEMSIZE (44)
#define MAXTHREAD (5)
#define MEMSIZE (sizeof(Memory))
#define NODESIZE (sizeof(Node))
#define MINWORDLEN (4)


void *thread_test(void *p){
	printf("mem_base: %p\nused_head: %p\nsize: %d\n", mem->mem_base, mem->used_head, mem->size);

	void *p = get_memory(48);
	void *p1 = get_memory(64);
	void *p2 = get_memory(32);

	Node *p_node = (Node *)((int)p - NODESIZE);
	//Node *p1_node = (Node *)((int)p1 - NODESIZE);
	Node *p2_node = (Node *)((int)p2 - NODESIZE);
	printf("p_node: %p; p_node->next: %p\n", p_node, p_node->next);
	//printf("p1_node: %p; p1_node->prev: %p;p1_node->next: %p.\n", p1_node, p1_node->prev, p1_node->next);
	printf("p2_node: %p; p2_node->prev: %p;p2_node->next: %p.\n", p2_node, p2_node->prev, p2_node->next);

	//printf("The address of p1: %p; size of p1: %d; offset of p1: %p.\n", p1, p1_node->size, p1_node);
	release_memory(&p1);
	/* Release p1_node */
	//p1_node = NULL;
	//printf("p1_node: %p\n", p1_node);
	printf("p_node->next: %p\n", p_node->next);
	printf("p2_node->prev: %p\n", p2_node->prev);
	void *p3 = get_memory(48);
	Node *p3_node = (Node *)((int)p3 - NODESIZE);
	printf("p3_node: %p; p3_node->prev: %p;p3_node->next: %p.\n", p3_node, p3_node->prev, p3_node->next);
	printf("p_node->next: %p; p2_node->prev: %p\n", p_node->next, p2_node->prev);
	printf("grow p3 memory.\n");
	printf("The address of p3 before: %p; size of p3 before: %d; offset of p3 before: %p.\n", p3, p3_node->size, p3_node);
	Node *tmp = grow_memory(44, &p3);
	if (tmp != NULL)
		p3 = tmp;
	/* Update p3_node */
	if (p3 != NULL)
		p3_node = (Node *)((int)p3 - NODESIZE);
	
	printf("The address of p3 now: %p; size of p3 now: %d; offset of p3 now: %p.\n", p3, p3_node->size, p3_node->data);
	release_memory(&p3);
	//release_memory(&p);
	release_memory(&p2);

	return NULL;
}
