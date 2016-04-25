/*
 * Austin Walter
 * Brennen Miller
 * CSCI 446 S16
 * Program 2
 * client.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 256 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

ssize_t readchunk( int sockfd, void *buf, size_t len );

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
//  int n = 1;

	if (argc != 4) {
	    fprintf(stderr,"client: incorrect number of inputs\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
  if (send(sockfd, argv[3], strlen(argv[3]), 0) == -1) {
      perror("send");
  }
  int nm = recv(sockfd, buf, 26, 0);
  buf[nm] = '\0';
  if(strcmp(buf,"server: all good          ") != 0)
  {
    printf("%s\n",buf);
    exit(1);
  }

  int file_desc = open(argv[3], O_CREAT|O_RDWR,0666);
  while((numbytes = readchunk(sockfd,buf,MAXDATASIZE-1)) > 0)
  {
    buf[numbytes] = '\0';
    write(file_desc, buf, numbytes);
  }
  close(file_desc);
  close(sockfd);

	return 0;
}

ssize_t readchunk( int sockfd, void *buf, size_t len ) {       
  return recv(sockfd, buf, len, MSG_WAITALL);  
}

