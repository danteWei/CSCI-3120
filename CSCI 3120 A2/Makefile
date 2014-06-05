CC=gcc
CFLAGS=-lpthread
EXECS=A2Exec
CLEAN_SRC= *.dSYM


all: compile clean

compile: Controller.c Model.c View.c GameOfLife.c list.c
	$(CC) $(CFLAGS) -g -std=c99 -o A2Exec Controller.c Model.c View.c GameOfLife.c list.c

clean: 
ifneq ($(wildcard $(CLEAN_SRC)),)
	-rm -rf $(CLEAN_SRC)
endif

spotless: clean
ifneq ($(wildcard $(EXECS)),)
	-rm -rf $(EXECS)
endif