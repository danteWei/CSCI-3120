#ifndef _common_h_
#define _common_h_

typedef struct {
	int num_threads;
	int size;
} Configuration;

typedef struct node {
	void *data;	//stores the allocated memory
	struct node *prev;
	struct node *next;
	int size;	//stores the size of the memory block, exclude the size of the node
	int pointers;	//stores the number of pointers points to it (exclude system pointers)
} Node;

typedef struct {
	void *mem_base;	//points to the initially allocated memory
	Node *used_head;	//points to the head node of the used memory block
	int size;	//stores the size of the allocated memory
} Memory;

void *get_memory(int size);
void *get_memory_while_locked(int size);
void *grow_memory(int size, void *p);
void release_memory_while_locked(void *p);
void release_memory(void *p);
int start_memory(int size);
void end_memory(void);
int parse_input(int argc, char ** argv, Configuration *config);
void *add_to_list(Node *position, int size);
void *thread_test(void *pv);

#endif
