#ifndef _common_h_
#define _common_h_

typedef struct {
	int num_threads;
	int size;
} Configuration;

typedef struct mem_block {
	void *data;
	struct mem_block *adjacent;
	int size;
	int used;
} Memory_block;

typedef struct node {
	void *data;	//stores the allocated memory
	struct node *prev;
	struct node *next;
	int size;	//stores the size of the memory block - 4
	//int used;	//1 if the memory block is in use, 0 otherwise
	int pointers;	//stores the number of pointers points to it (exclude system pointers)
} Node;

typedef struct {
	Node *head;
	int size;
} LinkedList;

typedef struct {
	void *mem_base;	//points to the initially allocated memory
	//Node *base_pointer;	//points to the whole memory block (exclude the first 4 bytes)
	Node *used_head;	//points to the head node of the used memory block
	//Node *unused_head;	//points to the head node of the unused memory block
	int size;	//stores the size of the allocated memory
} Memory;

void *get_memory(int size);
void *get_memory_while_locked(int size);
void *grow_memory(int size, void **p);
void release_memory_while_locked(void **p);
void release_memory(void **p);
int start_memory(int size);
void end_memory(void);
int parse_input(int argc, char ** argv, Configuration *config);
void *add_to_list(Node **position, int size);

#endif
