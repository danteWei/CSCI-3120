#ifndef _wordlist_h_
#define _wordlist_h_

#include "common.h"

typedef struct _word_node {
  struct _word_node *next;
  char *word;
  boolean marked;
} wordnode;

typedef struct {
  wordnode *head, *tail;
  int num_words;
} wordlist;

boolean wordlist_init( wordlist *list );
void wordlist_free( wordlist *list );

boolean wordlist_load( wordlist *list, FILE *infile ); 
char *wordlist_getword( wordlist *list, int position );
boolean wordlist_addword( wordlist *list, char *word );
int wordlist_totalchars( wordlist *list );
int wordlist_numwords( wordlist *list );

#endif

