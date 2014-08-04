Assignment 5 Documentation
===

The program implements a thread safe meory management system based on dynamic partitions that uses a "best fit" strategy. 

***

Usage:
------

When first time use this program, you need to compile the program, you can either use `gcc -g -m32 -o <executable name> Dynamic_Memory.c -lm -pthread` to compile the program (`-m32` tells the system to use 4 bytes for pointers and int), or you can simply type `make` in the command line (in this case the executable file name is "A5Exec"), if you want to delelete the executable file, use the command `make spotless`

To run the program, if the executable is called "A5Exec", then the command will be:

`./A5Exec -t <thread_number> -s <memory_size>`

- `-t`: specifies the thread number, if not specified, the program will use 1 thread

- `-s`: specifies the memory size, if not specified, the size will be 0 and the program will take the default size (1024)

**Note:** 

- The size is supposed to be a multiple of 4, if the input size is not a multiple of 4, the program will choose the next number that is a multiple of 4 as the input size.

- The program contains a hard coded `thread_test(void *p)` function, when executed, the threads will execute that function.

***
Data Structure
------

All data structures are defined in "common.h":

- Configuration: 

	- int num_threads: stores the number of threads a user wants to create, the maximum number is set to 5 and the minimum/default number is set to 1

	- int size: stores the number of memory bytes a user wants to allocate, the minimum number is set to 1024

- Node:
	
	- void *data: points to the memory location of this memory block where the data is stored

	- struct node *prev: points to the start of the previous memory block that is in use, NULL if this node is the head

	- struct node *next: points to the start of the next memory block that is in use, NULL is this node is the tail

	- int size: stores the number of bytes this memory block has, exclude the number of bytes used to store this node.

	- int pointers: 1 if this memory block is in use, 0 if this memory block is released and can be overwrite

- Memory:
	
	- void *mem_base: points to the start of the allocated memory (the base pointer)
	
	- Node *used_head: points to the head of a linked list in which the nodes point to the memory blocks that are in use

	- int size: stores the total size of the allocated memory that is  available to users

**Note:** You may have noticed that I do not have a pointer to keep track of the unused memory blocks, I'll explain why it is unnecessary in the later part.

***

Basic Functions:
------

- `void *get_memory(int size)`: allocate a memory partition with "size" bytes in it. Return a pointer to the memeory space or NULL if there is no space. **The actual size of the memory block is "size" + the size of the node**.

- `void *grow_memory(int size, void *p)`: try to grow the memory partition referenced by memory "p" so that it uses "size" by s with any extra space allocated at the end of the current memory block (e.g. if I call `p = grow_memory(92, *p)`, then the size of p will be 92 bytes after this call if there is enough space). The location of the old memory may change if the space after the current memory block is not enough. The function will return NULL if there is not enough in the entire memory.

- `void release_memory(void *p)`: release memory partition referenced by pointer "p" back to free space. (**This function will not update the pointers outside of this function, users will have to update those pointers themselves**)

- `int start_memory(int size)`: do whatever initialization. It should be only called once at the beginning of the program. It returns 1 if initiation succeeded, 0 otherwise.

- `void end_memory(void)`: do whatever task ending operations. It will also print out which memory block leaked if there is any. It should be only called once at the end of the program.

***

Algorithm
------

My algorithm is as follow:

- Get memory: 

	- check if the size is valid or not, if not, return NULL
	- check if there is enough space before the first memory block that is being used, if there is, add a memory block worth of (size + node_size) bytes, and make this new node the new used_list_head
	- if not, go through the used_list and find if there is any space between two blocks is enough, the computation is as follow: size_available_between_curr_and_next = next_node_address - curr_node_address - node_size - curr->size. **That is why it is unnecessary to keep track of the free memory space using a linked list**. Find the spot with the minimum size but enough size and add the memory block there
	- if no spot found, return NULL

- Release memory:

	- the memory is not actually "released", instead, all the information is still there, but the memory block is dereferenced from the used_list, and the pointers value is set to 0. This tells the program that these memory locations can be overwritten. 

- Grow memory:

	- if the size <= 0, release it
	- if the size is less than the size of the current data, just update current_node->size
	- else, first see if the extra space after the current block is enough or not, the computation is the same as mentioned above.
	- if enough, update curren_node->size, return the old data pointer
	- if not enough, first find a new location using get_memory function, if found one, release the old location using release_memory function and return the new data location.

- Start memory:

	- use a single `malloc` for the entire program
	- store the global variable (type Memory) at the very front of the memory we just allocated
	- if anything bad happened, return 0
	- else, return 1

- End memory:

	- if used_list_head is not NULL, that means there is (are) memory block(s) that is (are) not released, print them out and release them by following the linked list.
	- free the memory we allocated in the start_memory function

***

Threads
------

The entire program uses only one lock. The part of the program that needs concurrency control are: get_memory, release_memory, grow_memory. To make it easier for me to implement, I locked the whole function (or the whole memory) and unlock it right before the function ends. To avoid deadlocks, I implemented get_memory_while_locked and release_memory_while_locked functions, which are the same as the original ones but without locks, so that no two functions in the same thread will wait on the same lock.

I also set the maximum number of threads to be 5.





