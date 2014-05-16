EXEC = A1Exec

A1Exec: list.c Process.c Queue.c A1_Main.c
	gcc -g -o A1Exec list.c Queue.c Process.c A1_Main.c

clean:

spotless: clean
	-rm $(EXEC)
