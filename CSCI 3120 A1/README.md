Assignment 1 Documentation
===
The main file "A1_Main.c" implements the tasks listed on the assignment 1.
I could take PCB structure out and make it a separate class, but I didn't
because I think it is unnecessary.
***
The program will run on its own until it received a "Ctrl+C" command from
keyboard. Once it received the command, it will deallocate all memories it
created and then quit in 3 seconds.

USAGE:
--------
When first time use it, compile it first using "make" command to make sure the
executable is up to date, then involk the program using "./A1Exec" command.

I set the alarm interrupt to print "alarm start" every time it interrupts the
prgroam, so that we know it is interrupted. If you need to trigger any other
interrupt except SIGINT, use the following command "kill -s <signal name> pid"
to involk it.

Return Codes:
--------
There are some functions that return integers as their return codes, if the
return value is 1, it means the function is executed successfully, otherwise,
something wrong has occured. There are also some functions that do not have
return values, that is because it is unnecessary to do so.

Reviewing Codes:
--------
All of my codes are well-documented (at least I think I did...), so it should
not be hard to understand my program.

References:
--------
list.c, list.h and list.txt are written by Professor Michael McAllister, I did
make some changes to them (List_size() function, and added some attributes to
some of the data structures), but not much. I thank Professor Michael
McAllister for letting us use his codes. Some of the dieas in my program came
from parse.c and other codes provided by Michael McAllister.


Xinjing Wei

