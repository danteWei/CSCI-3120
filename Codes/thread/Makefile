
CC=gcc
CFLAGS=-lpthread
EXECS=thread1 thread2 thread3 threaded_list threaded_list2 threaded_list3 threaded_list4

all:  $(EXECS)

thread1: thread1.c
	$(CC) $(CFLAGS) -o thread1 thread1.c

thread2: thread2.c
	$(CC) $(CFLAGS) -o thread2 thread2.c

thread3: thread3.c
	$(CC) $(CFLAGS) -o thread3 thread3.c

threaded_list: threaded_list.c
	$(CC) $(CFLAGS) -o threaded_list threaded_list.c

threaded_list2: threaded_list2.c
	$(CC) $(CFLAGS) -o threaded_list2 threaded_list2.c

threaded_list3: threaded_list3.c
	$(CC) $(CFLAGS) -o threaded_list3 threaded_list3.c

threaded_list4: threaded_list4.c
	$(CC) $(CFLAGS) -o threaded_list4 threaded_list4.c

clean:

spotless: clean
	-rm $(EXECS)

