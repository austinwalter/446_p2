/*
 * 
 *
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT "5432"
#define MAX_LINE 256
#define MAX_PENDING 5

ssize_t readchunk( int s, void *buf, size_t len );

int main(int argc, char *argv[]){

	struct addrinfo hints;
	struct addrinfo *rp, *result;
	char buf[MAX_LINE];
	int s, new_s, n, length;
	int len;
	FILE *client_file;
	int test = 5;
	char item[MAX_LINE];
	int item_size;

	
	/* Build address data structure */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	/* Get local address info */
	if ((s = getaddrinfo(NULL, argv[1], &hints, &result)) != 0 )
	{
		fprintf(stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror(s));
		exit(1);
	}

	/* Iterate through the address list and try to perform passive open */
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 )
		{
			continue;
		}

		if (!bind(s, rp->ai_addr, rp->ai_addrlen))
		{
			break;
		}
		close(s);
	}
	if (rp == NULL)
	{
		perror("stream-talk-server: bind");
		exit(1);
	}
	if (listen(s, MAX_PENDING) == -1)
	{
		perror("stream-talk-server: listen");
		close(s);
		exit(1);
	}

	/* Wait for connection, then receive and print text */
	while(1)
	{	
		if ((new_s = accept(s, rp->ai_addr, &(rp->ai_addrlen))) < 0)
		{
			perror("stream-talk-server: accept");
			close(s);
			exit(1);
		}

        n = readchunk(new_s, buf, len);
        buf[n] = '\0';
        if((client_file = fopen(buf, "r")) == NULL)
        {
                fprintf(stderr, "Can't open input file from client\n");
                send(new_s, "no such file", 12, 0);
                close(new_s);
                close(s);
                return 1;
        }

        //length = sizeof(buf);
	while(fscanf(client_file, "%s", buf) != EOF)
	{	
	    length = sizeof(buf);
            while(test != length)   //send message until it sends the whole message
            {
                if(test == -1)
                {
                    printf("error in sending data\n");
                    return 1;
                }
		        test = send(new_s, buf, length, 0);
            }
            test = send(new_s, buf, length, 0);
	    }
	}

	freeaddrinfo(result);
	close(s);
	close(new_s);

	return 0;
}

ssize_t readchunk( int s, void *buf, size_t len ) {


     return recv(s, buf, len, MSG_WAITALL);

}
  
