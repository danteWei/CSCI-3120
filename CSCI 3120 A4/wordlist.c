#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include "common.h"
#include "wordlist.h"

#define MAX_WORD_SIZE (80)

/* The set of words is stored as a linked list of words.  Each node
   contains the word itself, a marker that says whether or not we have
   report this word in the past to a "getword" call, and th enext pointer. */


/* Initiatlize a word list.  The incoming list parameter must have
   space for a "wordlist" type already assigned. 

   Return True if the initialization worked and False if it failed.
*/

sem_t word_lock;  // A lock for geting the word without getting it twice

boolean
wordlist_init( wordlist *list ) {

if (sem_init(&word_lock, 0, 1) != 0){
  perror(strerror(errno));
  return False;
}

  boolean init_ok = False;

  if (list != NULL) {
    /* There is an actual list.  Initialize the data entries. */

    list->head = NULL;
    list->tail = NULL;
    list->num_words = 0;

    init_ok = True;


  }

  return init_ok;
}

/* Release all resources allocated to a word list.  The incoming parameter
   provides the list.  All the nodes in the list are release as is the 
   memory space allocated for each of the words in the list. */

void
wordlist_free( wordlist *list ) {
  wordnode *node;
  sem_destroy(&word_lock);
  if (list != NULL) {

    node = list->head;
    while (node != NULL) {
      /* Get rid of the first node. */

      list->head = node->next;

      /* Clean up what used to be the first node. */
      wordnode *tmp=node;
      free( tmp->word );
      free( tmp );

      node = list->head;
    }

    /* Clear the list itself. */

    list->head = NULL;
    list->tail = NULL;
    list->num_words = 0;
  }
}

/* Add a word to a word list.  Space is created to duplicate the word
   that is provided as a parameter. 

   The function allocates space for both a linked list node and for 
   a copy of the word being added.

   Return True if we were able to add the list and return False if we
   encoutered a problem in adding the word to the list.
*/

boolean
wordlist_addword( wordlist *list, char *word ) {
  boolean added = False;
  wordnode *node;
  int len;
  int i;

  /* Make sure that we have a real list and word to work with. */

  if ((list != NULL) && (word != NULL)) {
    len = strlen( word );

    node = (wordnode *)malloc( sizeof( wordnode ) );
    if (node != NULL) {
      node->word = (char *)malloc( (len+1) * sizeof( char ) );
      if (node->word != NULL) {

        /* We have space for the node. Fill it. */

        strncpy( node->word, word, len+1 );
        for (i = 0; i < len; i++) {
          node->word[i] = tolower(node->word[i]);
        }
        node->marked = False;
        node->next = NULL;

        /* Put the node at the end of the list. */

        if (list->tail == NULL) {
          list->head = node;
        } else {
          list->tail->next = node;
        }

        list->tail = node;
        list->num_words++;
        added = True;
      } else {
        added = False;
      }
    } else {
      added = False;
    }
  }
  return added;
}

/* Take all the words, one per line, from an input stream and add them
   in the order given to a list of words.  Those words are appended to the
   list.

   Return True if all the words were added and False if one or more words 
   were not entered or if other problems arose during the addition.
*/

boolean
wordlist_load( wordlist *list, FILE *infile ) {
  boolean loaded = False;
  char line[MAX_WORD_SIZE+1];
  int len;
  wordnode *node;

  if (list && infile) {
    line[MAX_WORD_SIZE] = '\0';
    loaded = True;

    /* Loop through all input, assuming one word per line. */

    while ((loaded == True) && fgets(line, MAX_WORD_SIZE, infile)) {

      /* Get rid of trailing carriage returns. */

      len = strlen(line);
      if (line[len-1] == '\n') {
        line[len-1] = '\0';
      }

      /* Actually store the word. */

      loaded = wordlist_addword( list, line );
    }
  }

  return loaded;
}

/* Give a list of words and a position in the list (between 0 and the list 
   size - 1), return the next "available" word in the list starting at
   that position.

   The function will only return a particular word once over all calls to
   the function.  A word is "available" if it has never been returned from
   this function.  We identify such words has having its "mark" field in the
   linked list node being False.

   If the search for a node goes completely around the list then the function
   will return a NULL pointer rather than go into an infinite loop.
*/

char *
wordlist_getword( wordlist *list, int position ) {

  /* Wait for the lock */
  if (sem_wait(&word_lock) != 0){
    perror(strerror(errno));
    exit(EXIT_FAILURE);
  }

  char *the_word = NULL;
  wordnode *node, *start;
  int nodecount;
  if ((list != NULL) && (position >= 0) && (position < list->num_words)) {

    /* Walk to the requested position. */
    list->tail->next = list->head;
    node = list->head;
    nodecount = 0;
    while ((node != NULL) && (nodecount < position)) {
      nodecount++;
      node = node->next;
    }

    if (node != NULL) {
      /* Keep walking until I find an unmarked node or I get back to this spot. */

      start = node;
      while ( (node->marked == True) && (node->next != start)) {
        node = node->next;

        /* At the end of the list, return to the start, like a circular list. */

        if (node == NULL) {
          node = list->head;
        }
      }

      /* Report what we've found and keep it from being returned again. */

      if (node->marked == False) {
        the_word = node->word;
        node->marked = True;
      }
    } else {
      /* I can only get here if my word count in the list is incorrect.  That shouldn't happen. */
      printf("word count incorrect\n");
    }
    list->tail->next = NULL;
  }

  /* Post the lock as soon as we get the word */
  if (sem_post(&word_lock) != 0){
    perror(strerror(errno));
    exit(EXIT_FAILURE);
  }
  return the_word;
}

/* Given a list of words, return the number of characters used in all
   the words of the list.  It gives an estimate of how many letters are
   needed to store just those words. */

int
wordlist_totalchars( wordlist *list ) {
  int total = 0;
  wordnode *node;

  if (list != NULL) {
    node = list->head;
    while (node != NULL) {
      if (node->word != NULL) {
        total += strlen( node->word );
      }
      node = node->next;
    }
  }
  return total;
}

/* Return the number of words currently stored in the list. */

int 
wordlist_numwords( wordlist *list ) {
  int num_words = 0;

  if (list != NULL) {
    num_words = list->num_words;
  }
  return num_words;
}
