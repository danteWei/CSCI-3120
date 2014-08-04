#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void add(Node *p, void *pv){

	Node *tmp = (Node *)((int)pv + sizeof(Node));
	*((int *)((int)pv + 90)) = 342;
	tmp->size = *(int *)((int)pv + 90);
	p->next = tmp;
	*((int *)((int)pv + 12)) = 3424;
	//p->size = (int *)((int)pv + 12);
	p->data = pv + 24;

	printf("offset: %d\n", (int)tmp - (int)p);
}

int main(){
	printf("%ld bytes per pointer\n", sizeof(Node *));


	void *pv = (void *) malloc(1024);
	Node *p = (Node *)pv;


	add(p, pv);
	printf("p->size: %d\n", (p->size));
	printf("size of NULL pointer is %ld\n", sizeof(NULL));
	printf("tmp->size: %d\n", (p->next->size));
	printf("Node address: %p; %p\n", (Node *)((int)pv), &(p->data));

	free(pv);

	return 0;
}