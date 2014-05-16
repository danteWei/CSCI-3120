
You'll find four examples of linked list code.  All of them start two
threads where each thread then tries to insert 10,000 nodes into a
linked list.

bad_list doesn't protect anything, so the race condition on the end of
the list causes us to lose nodes.

safe_list adds semaphores to the list code so that the whole adding
process is treated as a critical section.  Now, we always have 20,000
nodes in the list.

better_list goes one step further by making the critical section smaller.
Specifically, the call to malloc to create a new node doesn't need to
be in the critical section.  So, we can get a bit more concurrency.

granular_list goes to the extreme.  Rather than have one semaphore for
the whole linked list, the code now has one semaphore for each node.
This way, two threads can be searching the linked list at the same time
(so more concurrency).  However, when you run this version of the code,
things slow down because you're doing so many system calls for the
semaphore that you're wasting time.

