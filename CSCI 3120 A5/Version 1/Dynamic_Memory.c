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

/* a lock used to lock the unused/used linked list  */
sem_t unused_list_lock, used_list_lock;

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
	/*if (sem_init(&unused_list_lock, 0, 1) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (sem_init(&used_list_lock, 0, 1) == -1){
		perror(strerror(errno));
		exit(EXIT_FAILURE);
	}*/

	start_memory(config.size);

	printf("size of Memory: %ld\n", sizeof(Memory));
	printf("mem_base: %p\nused_head: %p\nunused_head: %p\nsize: %d\n", mem->mem_base, mem->used_head, mem->unused_head, mem->size);
	printf("data: %p\nused: %d\npointers: %d\n", mem->unused_head->data, mem->unused_head->used, mem->unused_head->pointers);

	end_memory();

	return 0;
}

/* allocate a memory partition with "size" bytes, return the pointer of that memory partition, NULL if out of memory */
void *get_memory(int size){
	void *memory_partition = NULL;
	Node *curr = mem->unused_head;
	Node *parent = NULL;
	Node *best_fit = NULL;

	/* Find the best unused spot for it */
	sem_wait(&unused_list_lock);
	while (curr != NULL){
		/* If a memory block for the first time */
		if (best_fit == NULL && curr->size >= size){
			best_fit = curr;
			parent = best_fit->prev;
			curr = curr->next;
			continue;
		}

		/* If find a better one */
		if (best_fit != NULL && curr->size >= size){
			if (curr->size < best_fit->size){
				best_fit = curr;
				parent = best_fit->prev;
			}
		}
		curr = curr->next;
	}

	/* Occupy the best fit spot */
	if (best_fit != NULL){
		Node *tmp = best_fit;
		/* not finished */
	}
	sem_post(&unused_list_lock);
	return memory_partition;
}

/* grow the memory referenced by p by "size" bytes */
void *grow_memory(int size, void *p){
	void *new_memory_patition = NULL;

	if (size > mem->size )
		/* If the size a thread is requesting is too large */
		printf("Cannot grow memory, the size is larger than the total memory available.\n");
	else{
		/* Else, we are good to go */

	}
	return new_memory_patition;
}

/* release memory partition referenced by p back to free space */
void release_memory(void *p){
	if (p != NULL){
		/* not finished */
	}
}

/* Intialization function */
int start_memory(int size){
	int all_ok = 1;

	/* if the size is too small to even put a pointer */
	if (size <= MINMEMSIZE){
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

	/* Initialize the memory data */
	Node *base = (Node *)((int)init_mem + MEMSIZE);
	/* Put the base pointer as the first 4 bytes of the memory */
	base->data = (void *)((int)init_mem + (MEMSIZE + NODESIZE));
	base->next = NULL;
	base->size = size - MEMSIZE - NODESIZE;
	base->used = 0;
	base->pointers = 0;

	/* initialize the memory */
	mem = init_mem;
	mem->mem_base = init_mem;
	//mem->base_pointer = base;
	mem->used_head = NULL;
	mem->unused_head = base;
	mem->size = base->size;

	return all_ok;
}

/* Do wahtever task-ending operations, print out memory leaks if there is any*/
void end_memory(void){
	/* Print memory leaks if there is any */
	Node *curr = mem->used_head;

	/* If used list is not empty */
	while (curr != NULL){
		if (curr->pointers > 0)
			printf("The memory (%d bytes) in address [%p] is not yet freed.\n", (curr->size + 4), curr->data);
		curr = curr->next;
	}

	/* Free all allocated memory */
	if (mem->mem_base != NULL)
		free(mem->mem_base);
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
              				if (atoi(argv[i]) > 0)
              					config->size = atoi(argv[i]);
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

