
#Thisnext linedefines arguments that are passed to all compilation steps

CFLAGS = -g -Wall
LDLIB = -lpthread

kerb : auth.o ticket.o service.o list.o pmessages.o client.o kerb.o
	gcc $(CFLAGS) -o kerb kerb.o auth.o ticket.o service.o client.o list.o pmessages.o $(LDLIB) 

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c
pmessages.o: pmessages.c pmessages.h pmessages_private.h
	gcc $(CFLAGS) -c pmessages.c
auth.o: auth.c auth.h pmessages.h list.h constants.h
	gcc $(CFLAGS) -c auth.c
ticket.o: ticket.c ticket.h pmessages.h list.h constants.h
	gcc $(CFLAGS) -c ticket.c
service.o: service.c service.h pmessages.h list.h constants.h
	gcc $(CFLAGS) -c service.c
client.o: client.c client.h pmessages.h list.h constants.h
	gcc $(CFLAGS) -c client.c
kerb.o: kerb.c client.h auth.h ticket.h service.h client.h list.h constants.h
	gcc $(CFLAGS) -c kerb.c

clean:
	-rm list.o pmessages.o ticket.o service.o auth.o client.o kerb.o

spotless: clean
	-rm kerb

