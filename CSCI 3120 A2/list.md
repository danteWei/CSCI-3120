Linked List sample code
-----------------------

The sample linked list code in list.c and list.h implement a
doubly-linked unordered list.  The list maintains head and tail
pointers to quickly access either end of the list.

The list lets you store a pointer to data.  The user of the linked
list is responsible for allocating the memory for the data that is
referenced by the pointer.  The list operations do nothing to the
data that is provided:  it does not duplicate it on insertion or
delete it when the list is destroyed.

Similarly, the user of the list is responsible for allocating the
space for the list metadata.  Users of the list must create space
for data of type List_t.  This space is then passed to all the
functions in the list.c file.

One restriction on the linked list is that it is not designed to
store a NULL pointer.

Usage
-----

The list is initialized with the List_init function and is destroyed
with the list_destroy function.  List_destroy releases the list
nodes and memory that the list.c code allocates.  It does _not_
release the user memory that is pointed to by the pointers supplied
to the functions to store data.

Return codes
------------
List_t functions that return a value return a int that should be
interpreted as a boolean value to say whether or not the list_operation
was successful.  A value of 0 represents False and any non-zero
value represents True or success.

When a list.c function is to return data to the caller, such as
with List_remove_head, the data is returned as a pass-by-reference
parameter to the function.

When no data is available to be returned, such as navigating to the
end of the linked list, the data pointer returns with the value
NULL.  As a consequence, it is not possible to store a NULL pointer
(or a value of 0) in the linked list.

Accessing the inside of the list
--------------------------------

Three functions give you access to the internals of the linked list:
- List_next_node -- for iterating over the list
- List_remove_in_context -- to remove a node found using List_next_node
- List_add_after -- to insert an item into the middle of the linked list 
  where the position was found using List_next_node

All three functions use a "context" parameter to let the list.c
functions remember where they are within the linked list.  List_next_node
changes the context variable while List_remove_in_context and
List_add_after just use the context.

To iterate through the linked list, begin with the context as a
NULL value.  Sample iteration code is:

List_t the_list;
void *context = NULL;
void *mydata;

/* Assume that the_list has been inialized and data has been added to it. */

while ( List_next_node( &the_list, &context, &mydata ) && (mydata != NULL)) {
  /* Do something with the data pointed to by mydata. */
}

Michael McAllister
