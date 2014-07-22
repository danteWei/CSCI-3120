CFLAGS = -g 
LDFLAGS = -lm

make_puzzle: make_puzzle.o grid.o wordlist.o
	gcc $(CFLAGS) -o make_puzzle make_puzzle.o grid.o wordlist.o $(LDFLAGS)

make_puzzle.o: make_puzzle.c grid.h wordlist.h common.h
	gcc $(CFLAGS) -c make_puzzle.c

grid.o: grid.c grid.h common.h
	gcc $(CFLAGS) -c grid.c

wordlist.o: wordlist.c wordlist.h common.h
	gcc $(CFLAGS) -c wordlist.c

clean:
	-rm make_puzzle.o grid.o wordlist.o

spotless: clean
	-rm make_puzzle

