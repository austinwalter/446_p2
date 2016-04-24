CFLAGS = -Wall -g -std=gnu99

all: server client

server: server.o
	gcc $(CFLAGS) -o server server.o

client: client.o
	gcc $(CFLAGS) -o client client.o

server.o: server.c
	gcc $(CFLAGS) -c server.c

client.o: client.c
	gcc $(CFLAGS) -c client.c

clean:
	rm *o server client
