/* This code is an updated version of the sample code from "Computer Networks: A Systems
 * Approach," 5th Edition by Larry L. Peterson and Bruce S. Davis. Some code comes from
 * man pages, mostly getaddrinfo(3). */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT "5432"
#define MAX_LINE 256

ssize_t readchunk( int s, void *buf, size_t len );

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *rp, *result;
    char *host;
    char buf[MAX_LINE];
    int s, n;
    int len;
    char *filename;
    int length;
    int i = 0, j = 0;
    FILE *fileinput;
	
    fflush(stdout);    
    printf("%d\n", argc);
    if (argc==4) 
    {
	host = argv[1];
        filename = argv[3];
    }
    else if(argc != 4)
    {
	printf("expecting 3 command line arguments, host/port/filename\n");
    }
    else 
    {
      fprintf(stderr, "usage: %s host\n", argv[0]);
      exit(1);
    }

  /* Translate host name into peer's IP address */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    if ((s = getaddrinfo(host, argv[2], &hints, &result)) != 0 ) {
        fprintf(stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror(s));
        exit(1);
    }

    /* Iterate through the address list and try to connect */
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 ) 
        {
            continue;
        }
        if (connect(s, rp->ai_addr, rp->ai_addrlen) == -1) 
        {
            printf("error connecting\n");
	    close(s);
            continue;
        }
        break;
    }
 
    if (rp == NULL) {
        perror("stream-talk-client: connect");
        exit(1);
    }

    freeaddrinfo(result);
    
    length = strlen(filename);
    int bytes_sent = 0;

    while(bytes_sent != length)
    {
        bytes_sent = send(s, filename, length, 0);
        if(bytes_sent == -1)
        {
            printf("Sending filename failed\n");
            return 1;
        }
    }
    printf("test after first send\n");
    fileinput = fopen(filename, "w");
    //n = readchunk(s, buf, len);
    buf[n] = '\0';
    
    while(n > 0)
    {
        if(n == len)
        {
             fprintf(fileinput, "%s", buf);
        }
        else if(n == -1)
        {
            printf("read error\n");
            close(s);
            return 1;
        }
        //n = readchunk(s, buf, len);
        buf[n] = '\0';
    }
    fclose(fileinput);
    close(s);

    return 0;

}

ssize_t readchunk( int s, void *buf, size_t len )
{

     return recv(s, buf, len, MSG_WAITALL);
    
}
