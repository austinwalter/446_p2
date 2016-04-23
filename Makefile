all: server client

server: server.o
	gcc server.o -o server

client: client.o
	gcc client.o -o client

t_server.o: t_server.c
	gcc -c -Wall -g t_server.c

t_client.o: t_client.c
	gcc -c -Wall -g t_client.c

clean:
	rm *o server client
