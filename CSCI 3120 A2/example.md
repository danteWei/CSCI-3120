- pthread_create
	- Create a thread
- pthread_join
	- Wait fro a thread to end
- pthread_self
	- Get my thread ID (not an integer)
- pthread_equal
	- Compare two thread IDs
- pthread_cancel
	- Stop a thread before it reaches the end of its function
- pthread_kill
	- Send a signal to a particular thread
- pthread_exit
	- Stop this thread but let other threads run


- Add `-lpthread` to your gcc compile line
	e.g. gcc -o test test.c -lpthread

- pthread_create(&id, &attr, function, &data);
	- &data must be a void pointer
- pthread_join(id, $data_pointer);
	- id is the thread id that this thread is waiting for 
	- threads that reutrn data should return a pointer to space created with `malloc`
- pthread_t pthread_self()
- int pthread_equal(id1, id2); (not necessary for assignment 2)
	- Return 0 if different, otherwise same
- int pthread_cancel(id)

- pthread_attr_init
	- Initialize the attribute data for a thread attribute structure
- pthread_attr_setscope(&attrib, PTHREAD_SCOPE_SYSTEM);
	- Set the scheduling parameters for a thread attribute structure

**Start a view with pthread**


- struct for Model
- struct for view
- struct for both Model & view
