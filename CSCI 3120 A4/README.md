Assignment 4 program Documentation
===

The program now spports multi threads. I add semaphores and pthread into the program. 

===

Usage
===

To compile the program, in the working directory where all program files are, type `make` in command line.

To use the program, check the usage in README.txt with additional command listed here:

- `-t n`: use n threads, n is a positive integer between 1 and 40. If n <= 0, then the program will use 1 thread, if n > 40, the program will use 40 threads. If missing n, the program will return a segmentation fault, so please check your command.

- to test the program, use the shared dictionary in UNIX: `head -n /usr/share/dict/words | ./make_puzzle <-g> <-s seed> <-t n>`

- to test the time: `time head -n /usr/share/dict/words | ./make_puzzle <-g> <-s seed> <-t n>`

- to test the time without counting the time for printing: `time head -n /usr/share/dict/words | ./make_puzzle <-g> <-s seed> <-t n> > /dev/null`

Thread
===

The program use `place_word()` function as the threaded function. All parameters in the original `palce_word()` function is now packed as a single parameter type called `thread_package`. The number of threads a user can create is limited due to the limitation on bluenose, it is currently set to be 40 threads, my tests (not in the test case file) showed that if the number of threads is greater than 45, then there is a possibility to fail.

Because of the issues listed below, I introduced a struct called `num_words_left` (consisting an integer and a lock) to synchronize the number of words left to fill in the word list. Now the program will synchronize the number of words left to put before it continues its next search, if it get one word, it decrement the number by one in a critical section.

The program stores all threads in a fixed-size array, the reason why I used a fixed-size array is that when I use a dynamic array to store over 4 threads, the program returns a "double free or corruption" error whenever it tries to join the fourth thread. The gdb shows that all threads are successfully terminated, and the fourth element in the dynamic array is not null. I emailed Professor Michael McAllister and he said this is the first he saw a bug like this. My guess is that something wrong with the C language itself or with the bluenose server.

The major changes I made for thread can be found in lines 306 - 348

Semaphores
===

### Algorithm ###

The algorithm is simple:

when the program is search for a spot for the word w with length l:

- if the program finds a set of cells start with (x, y), and with direction (xdir, ydir), and of length l:

	- if the cell can be used (empty and not locked), lock the cell using `sem_trywait()`;

	- if the cell is filled with a conflict letter or is already locked

		- Unlock all cells from (x, y) to the one before this cell along the directoin (xdir, ydir)

		- Discard this try

	- If the cell's letter agrees with the corresponding letter in w, then continue check the next cell

When the program finds better solution:

- Unlock the cells in previous opt solution

- make this solution the opt solution

After the program acutally puts the word in the grid (after 1000 tries)

- Unlock all cells in the opt solution

The reason why I did not use a read lock in the cell is that there is only one possibility that a cell is both locked and filled with some letter, that is, a thread is placing a word into the grid. That also means the cell will be unlocked, sooner or later. 

Suppose thread P1 is placing a word into the grid, it has already placed all the letters and is about to unlock those cells. Thread P2 comes in and find the letter in cell C1 agrees with a letter in P2's word, so P2 continues searching the next cell. P2 may then unlock C1 due to a conflict in the cell next to C1 before P1 unlcok C1, but that is alright, because:
	
	1. the cell C1 will be unlocked anyway;

	2. The unlock function will first check if the cell is locked or not, it will try to unlock the cell only if the cell is locked;

	3. My program will not lock a cell that already has a letter in it.

Therefore, there will be no conflict about when the program should unlock the cells after it placed a word in them. The program can either unlock the cell after putting in one letter, or unlock all cells all at once after putting in the whole word.

The reason why I explained the issue here is that I found it rather frustrating when trying to understand it. People may think, in their first impression, that the way I unlock cells may bring some issues. But the thing is I'VE CHECK IT. So I listed my proof here, to give the reader a good understanding.

### Concurrency Control ###

Also, I added a lock in `wordlist_getword()` to prevent the situation that two threads find the same word. The lock is a coarse-grained lock due to time efficiency. It is shown in class that if we lock/unlock every node in the linked list, the time spend on doing system calls is considerably large. Since the program does not technically search a word from the begining, the method that only locks a number of nodes is not easy to implement.

The program uses only `sem_trywait()` in finding a spot for a word, therefore, when search for cells, all threads never are never blocked by semaphore, and hence there should be no deadlock. There should not be any livelock since whenever a cell is locked, other threads will discard any tries containing this cell and start to try another one.

The changes I made are mainly in lines:

- make_puzzle.c
	
	- 161 - 276

- grid.c

	- 33 - 42
	- 83 - 89
	- 118 - 120
	- 277 - 318 (`grid_wordfits()` function)

- wordlist.c
	
	- 194 - 244 (`wordlist_getword()` function)

known Issues
===

Along with the limitation in bluenose and the unknown bug mentioned above. The program has the following issues, none of them is introduced by me:

- If the program is tested using multi-thread, and using a text file in which the words are typed in from keyboard, the function `wordlist_getword()` sometimes returns a null value and causes the program to hang up.

- The word in `head -n /usr/share/dict/words` may contain the character ".", which is also the EMPTY_GRID_CELL letter, the program may have difficulties deciding whether it is an empty cell or the cell is filled with "."

References
===

The original files, make_puzzle.c, common.h, grid.h, grid.c, wordlist.h, and wordlist.c are written by professor Michael McAllister. 

The files are edited by me so that it is now multi threaded and thread safe.


Xinjing Wei

