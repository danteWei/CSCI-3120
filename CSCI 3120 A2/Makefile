CC=gcc
CFLAGS=-lpthread
EXECS=A2Exec
CLEAN_SRC= *.dSYM
FILE = Controller.c Model.c View.c GameOfLife.c list.c

all: compile clean

compile: $(FILE)
	$(CC) $(CFLAGS) -std=c99 -g -o $(EXECS) $(FILE)

clean: 
ifneq ($(wildcard $(CLEAN_SRC)),)
	-rm -rf $(CLEAN_SRC)
endif

spotless: clean
ifneq ($(wildcard $(EXECS)),)
	-rm -rf $(EXECS)
endif