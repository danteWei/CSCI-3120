Assignment 2 Documentation
===
**Note: Codes are out of date, I'll update them after the due date**

The codes implement a Model, Controller \& View structure (MCV) for "Game of Life". 
The program uses Shared Memory Segments to share memory/files between threads. 

I created four C files, "GameOfLife.c", "Controller.c", "Model.c", and "View.c". And one single header file called "dataStructure.h". 
I put every function declarations and all structs in one header file so that it will be easier to manage.
***

Functions & data structures:
--------

There are 8 data structures (exclude data structures in list.c):
- MessageReady
	- Stores messageReady variable & errorSync variable
	- Determines if something is wrong during the synchronization
	- Determines if a new message is coming
- Grid
	- Stores the grid information
- CntrlToModel
	- Stores a message from Controller to Model
- ModeToCntrl
	- Stores a messaga from Model to Controller
- CntrlToView
	- Stores a message from Controller to a View
- Messages
	- Wraps the above three types of messages to one single structure
	- Makes it easier to pass through threads
- Legend
	- Stores legend information
	- In this case, it stores a character for "1"s, and a character for "0"s
- Package
	- Wraps all above strcures into one structure
	- Used to pass through threads
	- Also stores a linked list for all published grids created
	- Also stores a pointer points to the latest published grid, along with other information

***

There are 13 functions (exclude functions in list.c) in the whole program:
- Controller.c
	- int main(void);
		- The main function for the program
		- Initializes all initial data
		- Starts Controller thread and Model thread
	- void *Controller(void *p);
		- Function for Controller thread, takes in a shared package
		- Send instructions to Model and View threads
	- void read_input(pthread_t *Model, pthread_t *Views, Package *pkg);
		- Reads and parses user inputs
		- Does operations according to the user inputs
		- Updates the shared package
	- int exitThread(pthread_t *thread);
		- Terminates a thread
		- Returns 1 if successful, 0 otherwise
	- int exitProg(pthread_t *Model, pthread_t *Views);
		- Terminates Model and View threads
		- Returnss 1 if successful, 0 otherwise
	- int freeMem(Package *pkg);
		- Free allocated memories
		- Returns 1 if successful, 0 otherwise
- Model.c
	- void *modelPoll(void *p);
		- Function for Model thread
		- Sync every 2 seconds, updates the shared package
		- Does the calculation
	- int modelSync(Messages *msg);
		- Synchronizes model thread
		- Returns 1 if successful, 0 otherwise
	- int read_file(FILE *fp, Grid *g);
		- Reads a grid from a file into a new working grid
		- Returns 1 if succesful, 0 otherwise
	- int readConfig(char *fileName, Grid *g);
		- Reads a file for a new initial grid
		- Returns 1 if successful, 0 otherwise
- View.c
	- void * viewPoll(void *p);
		- Function for view threads
		- Sync every second
	- int viewSync(intit, int las);
		- Checks if the iteration number is updated
		- Returns 1 if the iteration number is updated, 0 otherwise
	- int write_to_file(Grid *g, char *type, int it, char *outputFileName, Legend *l, int *last);
		- Open/create a file for appending grids
		- Writes the new published grid to the file
		- Will use the legend specified by the user, will  use the default legend ". 1 *" if no legend is specified
		- Will write information according to the type of the view: "full" or "summary"
		- Returns 1 if successful, 0 otherwise
- GameOfLife.c
	- int game(Grid *g, int ***array);
		- Does the game of life calculation for one iteration
		- Stores the resulting grid in a 2-D array pointer
		- Returns 1 if successful, 0 otherwise

USAGE:
--------

**This program is designed to run only on UNIX based systems!**

To compile the program, just `cd` to the working directory and type `make` in command line. 
After the compilation, use `./A2Exec` to involk the program. Once the program is run, unless something bad happens, 
the program will listen to the keyboard inputs and do the operations until it receives an "end" input. Once it receives an "end", it will free all memories that were allocated and clean the shared memory segment before it terminates.

There are four input types:

- `start view <type> <file>`, where "type" is either "full" or "summary". This command starts a view thread for the specified elements. There will be at most 3 views.

- `view <X> legend <legend info>`, where _X_ is the view number that are being modified and "legend info" is a string that alternates a character and a number. E.g., "e 10 f 20 g" means that when printing the grid, print "e" if the number is less than 10 and print "f" is the number is between 10 and 20. Of course, in game of life, there will be only 1s and 0s. So the legend info format will always be "_\<some char\>_ 1 _\<some char\>_".

- `model <filename>` to have a mode switch its operation to use content of <filename> as a starting configuration for a new set of iterations on the gird.

- `end` to end to operation of the whole system cleanly.

When you type in your inputs, please be careful about the format, this program is *case-sensitive* and *whitespace-sensitive*.  Also when you input a "legend info", please try to avoid negative numbers and 0 is strictly forbidden.

A sample usage would be:

		$ cd path/to/working/directory
		$ make
		//program will be compiled here
		
		$ ./A2Exec
		TBA

Reviewing Codes:
--------
TBA

References:
--------

Xinjing Wei

