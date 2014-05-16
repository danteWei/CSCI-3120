
You can combine different .h and .c files for my_memory with test.c.

For the .h files:

my_memory0.h will use standard malloc and free
my_memory1.h will replace malloc and free your own functions (get_memory and release_memory) but needs your .c file to initialize the memory system.
my_memory2.h will replace malloc and free as in version 1, but replaces your "main" with a new main function that calls the init code
my_memory3.h does what version 2 does but calls the get_memory function that can also report the file and line number where the malloc call is made.

For the .c files:

my_memory.c is a basic implementation of get_memory and release_memory that falls back to malloc and free
my_memory2.c tracks how many bytes are requested from malloc and realloc across the program
my_memory3.c tracks the bytes requested and how many are still allocated when the program ends
my_memory4.c is like version 3, but it shows that you can store an arbitrary data structure before your memory blocks to track information.

