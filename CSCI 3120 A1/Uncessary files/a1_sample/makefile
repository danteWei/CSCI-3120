
#Thisnext linedefines arguments that are passed to all compilation steps

CCFLAGS = -g -Wall

simulate: list.o parse.o
	gcc $(CCFLAGS) -o simulate list.o parse.o

list.o: list.c list.h
	gcc $(CCFLAGS) -c list.c

parse.o: parse.c 
	gcc $(CCFLAGS) -c parse.c

clean:
	-rm list.o parse.o simulate

