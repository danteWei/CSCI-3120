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
#define MAXTHREAD (10)
#define MEMSIZE (sizeof(Memory))
#define NODESIZE (sizeof(Node))
#define MINWORDLEN (4)


/* Global variable that keeps track of a base address of a block of memory */
static Memory *mem;

/* a lock used to lock the linked list  */
sem_t mem_lock;

/* Main function */
int main(int argc, char **argv){
	/* Configuration variable for inputs */
	Configuration config;
	
	/* Parse in inputs */
	if (parse_input(argc, argv, &config) == 0){
		printf("Fail to parse input or one or more inputs are invalid. \nPlease check your input and try again.\n");
		exit(EXIT_FAILURE);
	}

	/* Initialize list locks */
	if (sem_init(&mem_lock, 0, 1) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(start_memory(config.size) == 0)
		exit(EXIT_FAILURE);

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

	end_memory();

	return 0;
}

/* Add a node to the speficified position, returns a pointer points to the data */
void *add_to_list(Node **position, int size){
	Node *memory_partition_node;
	if (position == NULL){
		/* Create a node as the head of the used list */
		memory_partition_node = (Node *)((int)(mem->mem_base) + MEMSIZE);

		memory_partition_node->next = NULL;
		memory_partition_node->prev = NULL;

		/* Make the Node the used list head */
		mem->used_head = memory_partition_node;
	}
	else{
		/* Add the node next to position */
		memory_partition_node = (Node *)((int)(*position) + (*position)->size + NODESIZE);
		/* Update pointers */
		memory_partition_node->next = (*position)->next;
		memory_partition_node->prev = (*position);

		/* If position is not the tail */
		if ((*position)->next != NULL)
			(*position)->next->prev = memory_partition_node;
		(*position)->next = memory_partition_node;
	}
	/* Assign values to the Node */
	memory_partition_node->data = (void *)((int)memory_partition_node + NODESIZE);
	memory_partition_node->size = size;	
	memory_partition_node->pointers = 1;

	return memory_partition_node->data;
}

/* allocate a memory partition with "size" bytes, return the pointer of that memory partition, NULL if out of memory */
void *get_memory(int size){
	void *memory_partition = NULL;
	Node *curr = mem->used_head;
	Node *best_fit = NULL;

	/* Find the best unused spot for it */
	if (sem_wait(&mem_lock) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (size > (mem->size - NODESIZE)){
			/* If the requested size is too large */
			printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
			if (sem_post(&mem_lock) == -1){
				perror(strerror(errno));
				exit(EXIT_FAILURE);
			}
			return NULL;
	}
	/* Find the best unused spot for it */

	/* If size is not a multiple of 4, round it to the next integer that is a multiple of 4 */
	if (size % 4 != 0){
		printf("The requested size (%d) is not a multiple of 4, program will allocate %d bytes instead of %d bytes.\n", size, (size + (4 - (size % 4))), size);
		size = size + (4 - (size % 4));
	}

	/* If no memory is used */
	if (curr == NULL){
		if (size > (mem->size - NODESIZE)){
			/* If the requested size is too large */
			printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
			if (sem_post(&mem_lock) == -1){
				perror(strerror(errno));
				exit(EXIT_FAILURE);
			}
			return NULL;
		}
		else{
			/* If there is enough space */
			memory_partition = add_to_list(NULL, size);
		}
	}
	else{
		/* If some memory is used */
		if (curr->next == NULL){
			/* If there is only one memory partition in use */
			if (size > (mem->size - NODESIZE - curr->size)){
				/* Not enough space */
				printf("Not enough space.\n");
				if (sem_post(&mem_lock) == -1){
					perror(strerror(errno));
					exit(EXIT_FAILURE);
				}
				return NULL;
			}
			else{
				memory_partition = add_to_list(&curr, size);
			}
		}
		else{
			while (curr != NULL){
				/* If find a memory block for the first time */
				if (best_fit == NULL && (((int)(curr->next) - curr->size - NODESIZE - (int)(curr)) >= size)){
					if (curr->next != NULL){
						best_fit = curr;
						curr = curr->next;
					}
					else{
						if ((mem->size - ((int)curr) + MEMSIZE - NODESIZE - curr->size) < size){
							printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
							if (sem_post(&mem_lock) == -1){
								perror(strerror(errno));
								exit(EXIT_FAILURE);
							}
							return NULL;
						}
						else{
							best_fit = curr;
							curr = curr->next;
						}

					}
					continue;
				}

				/* If find a better one */
				if (best_fit != NULL && (((int)(curr->next) - curr->size - NODESIZE - (int)curr)) >= size){
					if ( curr->next != NULL && ((int)(curr->next) - curr->size - NODESIZE - (int)(curr)) < ((int)(best_fit->next) - (int)(best_fit) - NODESIZE - best_fit->size) )
						best_fit = curr;
					if (curr->next == NULL &&((mem->size - ((int)curr) + MEMSIZE - NODESIZE - curr->size) < size)){
						printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
						if (sem_post(&mem_lock) == -1){
							perror(strerror(errno));
							exit(EXIT_FAILURE);
						}
						return NULL;
					}
					if (curr->next == NULL &&((mem->size - ((int)curr) + MEMSIZE - NODESIZE - curr->size) > size)){
						best_fit = curr;
					}
				}
				curr = curr->next;
			}
			/* Occupy the best fit spot */
			if (best_fit != NULL)
				memory_partition = add_to_list(&best_fit, size);
		}
	}
	if (sem_post(&mem_lock) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	return memory_partition;
}

/* An unlock version of get_memory function */
void *get_memory_while_locked(int size){
	void *memory_partition = NULL;
	Node *curr = mem->used_head;
	Node *best_fit = NULL;

	if (size > (mem->size - NODESIZE)){
			/* If the requested size is too large */
			printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
			return NULL;
	}
	/* Find the best unused spot for it */

	/* If size is not a multiple of 4, round it to the next integer that is a multiple of 4 */
	if (size % 4 != 0){
		printf("The requested size (%d) is not a multiple of 4, program will allocate %d bytes instead of %d bytes.\n", size, (size + (4 - (size % 4))), size);
		size = size + (4 - (size % 4));
	}

	/* If no memory is used */
	if (curr == NULL){
		if (size > (mem->size - NODESIZE)){
			/* If the requested size is too large */
			printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
			return NULL;
		}
		else{
			/* If there is enough space */
			memory_partition = add_to_list(NULL, size);
		}
	}
	else{
		/* If some memory is used */
		if (curr->next == NULL){
			/* If there is only one memory partition in use */
			if (size > (mem->size - NODESIZE - curr->size)){
				/* Not enough space */
				printf("Not enough space.\n");
				return NULL;
			}
			else{
				memory_partition = add_to_list(&curr, size);
			}
		}
		else{
			while (curr != NULL){
				/* If find a memory block for the first time */
				if (best_fit == NULL && (((int)(curr->next) - curr->size - NODESIZE - (int)(curr)) >= size)){
					if (curr->next != NULL){
						best_fit = curr;
						curr = curr->next;
					}
					else{
						if ((mem->size - ((int)curr) + MEMSIZE - NODESIZE - curr->size) < size){
							printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
							return NULL;
						}
						else{
							best_fit = curr;
							curr = curr->next;
						}

					}
					continue;
				}

				/* If find a better one */
				if (best_fit != NULL && (((int)(curr->next) - curr->size - NODESIZE - (int)curr)) >= size){
					if ( curr->next != NULL && ((int)(curr->next) - curr->size - NODESIZE - (int)(curr)) < ((int)(best_fit->next) - (int)(best_fit) - NODESIZE - best_fit->size) )
						best_fit = curr;
					if (curr->next == NULL &&((mem->size - ((int)curr) + MEMSIZE - NODESIZE - curr->size) < size)){
						printf("the size that is requested is too large! The program will not allocate any memory to this request.\n");
						return NULL;
					}
					if (curr->next == NULL &&((mem->size - ((int)curr) + MEMSIZE - NODESIZE - curr->size) > size)){
						best_fit = curr;
					}
				}
				curr = curr->next;
			}
			/* Occupy the best fit spot */
			if (best_fit != NULL)
				memory_partition = add_to_list(&best_fit, size);
		}
	}
	return memory_partition;
}

/* grow the memory referenced by p by "size" bytes */
void *grow_memory(int size, void **p){
	void *new_memory_patition = NULL;
	Node **p_node;
	*p_node = (Node *)((int)(*p) - NODESIZE);
	if (sem_wait(&mem_lock) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (*p_node != NULL){

		int requested_size = (*p_node)->size + size;	//the total size it will have if the memory is growed
		int curr_available = (int)((*p_node)->next) - (int)(*p);	//the size of the memory segment that is available for this location

		/* If there is enough space for current location, update the memory size */
		if (requested_size <= curr_available){
			(*p_node)->size = requested_size;
			new_memory_patition = *p;
		}
		/* If not, try to find another location for if */
		else{
			new_memory_patition = get_memory_while_locked(requested_size);
			/* If found one, release the old memory segment */
			if (new_memory_patition != NULL){
				release_memory_while_locked(p);
				/* release the node pointer */
				(*p_node)->next = NULL;
				(*p_node)->prev = NULL;
				(*p_node)->size = 0;
				(*p_node)->data = NULL;
				(*p_node)->pointers = 0;
				*p_node = NULL;
			}
			/* If no space for the request */
			else
				printf("Fail to grow memory, not enough space.\n");

			
		}
	}
	if (sem_post(&mem_lock) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	return new_memory_patition;
}

/* release one memory block referenced by p back to free space */
void release_memory(void **p){
	if (sem_wait(&mem_lock) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (p != NULL && *p != NULL){
		/* Find the node points to this memory partition */
		Node **p_node;
		*p_node = (Node *)((int)(*p) - NODESIZE);
		/* release memory */
		int i;
		for (i=0; i<(*p_node)->size; i=i+4){
			void *tmp = (void **)((int)(*p) + 4);
			*p = NULL;
			p = &tmp;
		}
		/* If n is not the first node */
		if ((*p_node)->prev != NULL)
			(*p_node)->prev->next = (*p_node)->next;
		/* if n is not the last node */
		if ((*p_node)->next != NULL)
			(*p_node)->next->prev = (*p_node)->prev;
		(*p_node)->data = NULL;
		(*p_node)->pointers = 0;
		(*p_node)->next = NULL;
		(*p_node)->prev = NULL;
		*p_node = NULL;
		if ((*p_node) != NULL){
			printf("Fail to release memory, system will terminate.\n");
			exit(EXIT_FAILURE);			
		}
	}
	if (sem_post(&mem_lock) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
}
/* An unlock version of release_memory function */
void release_memory_while_locked(void **p){
	if (p != NULL && *p != NULL){
		/* Find the node points to this memory partition */
		Node **p_node;
		*p_node = (Node *)((int)(*p) - NODESIZE);
		/* release memory */
		int i;
		for (i=0; i<(*p_node)->size; i=i+4){
			void *tmp = (void **)((int)(*p) + 4);
			*p = NULL;
			p = &tmp;
		}
		/* If n is not the first node */
		if ((*p_node)->prev != NULL)
			(*p_node)->prev->next = (*p_node)->next;
		/* if n is not the last node */
		if ((*p_node)->next != NULL)
			(*p_node)->next->prev = (*p_node)->prev;
		(*p_node)->data = NULL;
		(*p_node)->pointers = 0;
		(*p_node)->next = NULL;
		(*p_node)->prev = NULL;
		(*p_node) = NULL;
		if ((*p_node) != NULL){
			printf("Fail to release memory, system will terminate.\n");
			exit(EXIT_FAILURE);			
		}
	}
}
/* Intialization function */
int start_memory(int size){
	int all_ok = 1;

	/* if the size is too small to even put a pointer */
	if (size < MINMEMSIZE){
		printf("The size is too small, please enter a size larger than %d\n", MINMEMSIZE);		
		return 0;
	}
	/* Initialize memory block */
	void *init_mem = (void *) malloc(size);
	if (init_mem == NULL)
	{
		/* If fail to initialize memory block */
		printf("Fail to initialize memory.\n");
		all_ok = 0;
	}

	/* initialize the memory */
	mem = init_mem;
	mem->mem_base = init_mem;
	//mem->base_pointer = base;
	mem->used_head = NULL;
	//mem->unused_head = base;
	mem->size = size - MEMSIZE;
	return all_ok;
}

/* Do wahtever task-ending operations, print out memory leaks if there is any*/
void end_memory(void){
	/* Print memory leaks if there is any */
	int count = 0;
	Node *curr = mem->used_head;

	/* If used list is not empty */
	while (curr != NULL){
		if (curr->pointers > 0){
			count++;
			printf("The memory (%d bytes) in address [%p] is not yet freed (memory leak). \n\tThe address of the node pointer of this memory is [%p]. \n\n", curr->size, (curr->data), curr);
		}
		curr = curr->next;
	}

	if (count == 0)
		printf("No memory leak detected, program can exit cleanly.\n\n");
	/* Free all allocated memory */
	if (mem->mem_base != NULL){
		free(mem->mem_base);
		mem->used_head = NULL;
		mem->size = 0;
	}
}

int parse_input(int argc, char ** argv, Configuration *config){
	int all_ok=1;
	int i;
	if (config != NULL){

		/* Define default values */
		config->num_threads = 1;
		config->size = 0;

		/* See if any parameter values are now overridden */

		i = 1;
		while (i < argc){
			if ((argv[i][0] == '-') && (strlen(argv[i]) > 1)){
				switch(argv[i][1]){
					case 't':
						/* Multi-thread */
						if (i < argc){
							i++;

							/* Check if the input thread number is valid or not */
							if (atoi(argv[i]) <= MAXTHREAD && atoi(argv[i]) > 0)
                				config->num_threads = atoi(argv[i]);
              				else if (atoi(argv[i]) <= 0)
                				printf("The number of threads you want to create is not a positive integer.\nProgram will use 1 thread.\n");
              				else {
                				printf("The number of threads you want to create exceeds the limit (%d threads).\n Program will use %d threads.\n", MAXTHREAD, MAXTHREAD);
                				config->num_threads = MAXTHREAD;
              				}
              			}
            			else{
              				config->num_threads = 1;
              				printf("Number of threads is set to 1\n");
            			}
              			break;

              		case 's':
              			/* Memory size */
              			if (i < argc){
              				i++;

              				/* Check if the input memory size is valid */
              				if (atoi(argv[i]) > 0 && (atoi(argv[i]) >= MINMEMSIZE)){
              					if (atoi(argv[i]) % 4 == 0)
              						config->size = atoi(argv[i]);
              					else{
              						int re = atoi(argv[i]) % 4;
              						printf("The size you entered is not a multiple of 4.\nThe program will use the next integer that is a multiple of 4 (%d) as the input size.\n", atoi(argv[i]) + (4 - re));
              						config->size = atoi(argv[i]) + (4 - re);
              					}
              				}
              				else if (atoi(argv[i]) < MINMEMSIZE){
              					printf("The memory size you entered is too small, please enter an integer larger than %d\n", MINMEMSIZE);
              					all_ok = 0;
              				}
              				else{
              					printf("The memory size %s is an invalid size.\n", argv[i]);
              					all_ok = 0;
              				}
              			}
              			break;

              		default:
              			/* Ignore the parameter */
              			break;
				}
			}
			i++;
		}
	}
	return all_ok;
}

