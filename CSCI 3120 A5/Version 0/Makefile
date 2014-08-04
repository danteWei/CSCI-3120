CC = gcc
CFLAGS = -g -m32 -o
LDFLAGS = -lm -pthread
EXECS = A5Exec
FILE = *.c

all: compile clean

compile: $(FILE)
	$(CC) $(CFLAGS) $(EXECS) $(FILE) $(LDFLAGS)

clean: 
	-rm -rf *.dSYM

spotless: 
	-rm $(EXECS)