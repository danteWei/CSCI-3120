CC = gcc
CFLAGS = -g -m32 -o
LDFLAGS = -lm -pthread
EXECS = A5Exec
FILE = *.c
TEST_FILE = test.c
TEST_OUT = test.out

all: compile clean

compile: $(FILE)
	$(CC) $(CFLAGS) $(EXECS) $(FILE) $(LDFLAGS)

clean: 
	-rm -rf *.dSYM

spotless: 
	-rm $(EXECS)

test:
	$(CC) $(CFLAGS) $(TEST_OUT) $(TEST_FILE)