
#ifndef _my_memory_h_
#define _my_memory_h_

#ifndef _DONT_SUBSTITUTE

#define malloc(A) get_memory((A))
#define free(A) release_memory((A))
#define realloc(A, B) grow_memory((A), (B))
#endif

void *get_memory( int size );
void release_memory( void *p );
void *grow_memory( void *p, int new_size );
int init_memory( int size );
void end_memory( void );

#endif

