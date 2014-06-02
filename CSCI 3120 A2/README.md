Assignment 2 Documentation
===
The codes implement a Model, Controller & View structure (MCV) for "Game of Life". 
The program uses Shared Memory Segments to share memory/files between threads. 

There are three files, "Controller.c", "Model.c", and "View". And one single header file called "dataStructure.h". 
I put every function declaration and all structs in one header file so that is easier to manage.
***
TBA

USAGE:
--------

**This program is designed to run only on UNIX based systems!**

To compile the program, just `cd` to the working directory and type `make` in command line. 
After the compilation, use `./A2Exec` to involk the program. Once the program is run, unless something bad happens, 
the program will listen to the keyboard inputs until it receives an "end" input.

There are four input types:

- `start view <type> <file>`, where "type" is either "full" or "summary". This command starts a view thread for the specified elements. There will be at most 3 views at most.

- `view <X> legend <legend info>`, where _X_ is the view number that are being modified and "legend info" is a string that alternates a character and a number. E.g., "e 10 f 20 g" means that when printing the grid, print "e" if the number is less than 10 and print "f" is the number is between 10 and 20. Of course, in game of life, there will be only 1s and 0s. So the legend info format will always be "_\<some char\>_ 1 _\<some char\>_".

- `model <filename>` to have a mode switch its operation to use content of <filename> as a starting configuration for a new set of iterations on the gird.

- `end` to end to operation of the whole system cleanly.

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
