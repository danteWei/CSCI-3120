Overview
--------

This program accepts a list of words (one per line) from the keyboard
and produces a "word search" puzzle of the words.  The current
version just prints the solution to the puzzle and prints a '.' in
the puzzle positions that should be filled with a random letter.

The program goes to some effort to pack the words as tightly together
as it can.

The program requires that each "word" be less than 80 characters
and be comprised only of letters.

Data structures
---------------

The program uses two data structures.  The first data structure is
a singly-linked list to store all the words that will be added to
the puzzle.  Each node in the linked list also has a "mark" field
that lets us remember when we have placed the word into the puzzle.

The second data structure is the puzzle grid.  That data structure
is a two-dimensional grid of a "gridcell" data type, which is just
a letter.  To make the creation of the puzzle easier, we create an
oversized puzzle in which to place the words and then trim off the
unused parts of the grid once all the words are in place.  Consequently,
the grid has fields "rows" and "cols" to say how big the grid's
space is and then separate "x_min", "x_max", "y_min", and "y_max"
fields to say where the parts of the grid in use appear.

When creating the grid, we estimate how much space we may need for
all the words as a function of how many letters eventually need to
be put into the puzzle.  Experimentally, we seem to be able to get
a 60% load factor (so 60% of the effective grid contains some letter
from an input word).  We use a conservative value for this load
factor (20%) to scale our current grids.

Conceptually, the (0,0) corner of the grid is in the upper left
corner of what is displayed on the screen as the final puzzle.  When
looking at the indices of the grid, the first array index is the
x-coordinate (left-to-right) and the second array index is the y
coordinate (top-to-bottom).

Algorithm
---------

The puzzle creation proceeds by picking random locations for a word to
start and a random direction in which to be read.  It tries to find many
positions where the word could go and selects the location for the word
with the following charactertics:
  - among all candidate locations, the final location uses the most
    letters already in the grid fo the word
  - secondarily, we prefer a location that starts closest to the centre
    of the grid as a means to have a dense packing for the puzzle.

More specifically, the placement algorithm operates as follows:

  1. pick a random word to insert into the puzzle
  2. start by searching a subset of the grid.  That subset is a square
     centred on the centre of the over all grid and whose size is 1 
     character bigger than the word to insert.
  3. repeatedly choose a starting (x,y) position and direction (xdir, ydir)
     for the word that fits within this subset of the grid.  The repetition
     continues until we find MAX_TRIES (1000) possible word placements that 
     are at least as good as what we have so far in terms of overlap with
     the existing grid.

     3.1  Periodically (every GROW_RATE (100) placement attempts), grow the
          size of our search subset by a row and column to give us a chance to
          get away from the centre of the grid.

  4. Put the word in the best placement that we found (at coordinates
     (x_opt, y_opt) in direction (xdir_opt, ydir_opt) ... using "opt" as
     our "optimal" placement indicator).

Code organization
-----------------

The mainline of the program and all the logic for deciding when we have
tried enough possible positions for a word are in make_puzzle.c.

The list of words an all operations that manage that list is in wordset.c.

All interaction with the grid is accomplished through the functions in grid.c.

Last, "common.h" contains definitions that every file might need.  In this
case, it defines a boolean data type.

The directory includes a makefile to show how to create the program.
Entering the UNIX command "make" on bluenose will compile the whole
program to an executable called make_puzzle   If you enter "make clean" 
then all of the object files are removed.  If you enter "make spotless" 
then all files that can be derived will be removed.

Command line arguments
----------------------

When invoking the program from the command line, you can provide the following
flags:

  -g        prints just the solution grid only.  Cells not used in a word are
            displayed as '.'

  -s n      uses the integer "n" as the seed for the random number generator

  -o name   sends all output to the file called "name" rather than to stdout

The input words are taken from stdin (the keyboard).  To get the words from
a file, use input redirection.  If the words are in the file "foo" then start
the program as

  ./make_puzzle < foo

To use the number 42 as the random seed, sending output to the file
"bar", printing only the solution, and getting the words for the
puzzle from the file called "foo", start the program as

  ./make_puzzle -g -s 42 -o bar < foo


