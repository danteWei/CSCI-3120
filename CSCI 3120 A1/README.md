Assignment 1 Documentation
===
The main file "A1_Main.c" implements the tasks listed on the assignment 1.
I could take PCB structure out and make it a separate class, but I didn't
because I think it is unnecessary. [This is the link that explains how list work](list.md).
***
The program will run on its own until it received a `Ctrl+C` command from
keyboard. Once it received the command, it will deallocate all memories it
created and then quit in 3 seconds.

**Please note that I do have a new queue, and the input processes will only be
moved to ready queue when the alarm interrupt is triggered!**

USAGE:
--------
**This program is designed to run only on UNIX based system!** It may be ok to run the program on a LINUX based system, but I never test it.

When first time use it, compile it first using `make` command to make sure the
executable is up to date, then invoke the program using `./A1Exec` command.

I set the alarm interrupt to print "alarm start" every time it interrupts the
prgroam, so that we know it is interrupter, but now I commented it out, please
fell free to add it back if you want to. If you need to trigger any other
interrupt except SIGINT, use the following command `kill -s <signal name> pid`
to invoke it. If you want to find the process ID, use the following command: 
`ps ax | grep <program name>`. For example, I use `./A1Exec` to invoke my program, 
then if I want to find the pid for my program, I'll type in `ps ax | grep ./A1Exec` 
**on another console window**.

If you worry that the data will be lost when the alarm interrupt interrupts the program while you
are typing in a process, don't worry about it. all what you typed will still
be read into the program even when an alarm interrupt is triggered in the
process.

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
make some changes to them (`List_size()` function, I was using it for debugging,
so I did not implement `isEmpty()` function, and I also added some attributes to
some of the data structures), but not much. I thank Professor Michael
McAllister for letting us use his codes. Some of the dieas in my program came
from parse.c and other codes provided by Michael McAllister.


Xinjing Wei


