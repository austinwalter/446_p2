all: server client

server: server.o
	gcc server.o -o server

client: client.o
	gcc client.o -o client

t_server.o: server.c
	gcc -c -Wall -g server.c

t_client.o: client.c
	gcc -c -Wall -g client.c

clean:
	rm *o server client
