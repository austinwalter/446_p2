/*
 *Brennan Miller
 *Austin Walters
 *CSCI 446
 *Programming Assignment 1
*/

#define SERVER_NAME "www.ecst.csuchico.edu"
#define SERVER_PORT "80"
#define REQUEST "GET /~ehamouda/file.html HTTP/1.0\n\n"
#define MAX_LINE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
                const char *service,  // e.g. "http" or port number
                const struct addrinfo *hints,
                struct addrinfo **res);
/*
 * Arguments: same as recv(2)
 * Return:-1 on error or bytes received
 *
 * Receive len bytes of data into array buf from socket sockfd. Always receive len bytes
 * of data unless the socket closes. The return value will fall into one of three cases:
 *   (1) on success the return value equals len
 *   (2) on error the return value equals -1
 *   (3) if the socket closes without receiving len bytes, then return the number
 *       actually received, which may be zero
 */
ssize_t readchunk( int sockfd, void *buf, size_t len );

int main( int argc, char *argv[] ) 
{
    char tag[3];   //the type of tag is being looked for
    char is_tag[3];       //Used for checking if the file contains the specified tag
    strcpy(tag, argv[2]);
    struct addrinfo hints, *res;
    int sockfd;
    int num_tags = 0;
    int n = 1;
    int len = atoi(argv[1]);    //The length of the data chunk to be recieved
    char buf[MAX_LINE];
// first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(SERVER_NAME, SERVER_PORT, &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd == -1) {
        printf("error sock\n");
        return -1;
    }

    if(connect(sockfd, res->ai_addr, res->ai_addrlen) == -1){ //Connect to host using addrinfo
        printf("error connect\n");
        return -1;
    }

    int length, bytes_sent = 0;
    length = strlen(REQUEST);
 
    while(bytes_sent != length)  //try to send the message until the entire message has been sent
    {
        bytes_sent = send(sockfd, REQUEST, length, 0); 
        if(bytes_sent == -1)
        {
            printf("Sending Request for data failed \n");
            return 1;
        }
    }
    int i = 0, j = 0;
    bzero(buf, 0);
    n = readchunk(sockfd, buf, len);
    
    //printf("%s", buf);
    printf("%s\n", tag);
    while(n > 0)
    { 
        if(n == len)
        {
            for(i = 0; i < len; i++)
            {
                is_tag[0] = buf[i];
                is_tag[1] = buf[i+1];
                is_tag[2] = buf[i+2];
                if(strcmp(tag, is_tag) == 0)
                {   
                    num_tags++;
                }
            }
        }
        else if(n < len)
        {
            i = 0;
            for(i = 0; i < len; i++)
            {
                is_tag[0] = buf[i];
                is_tag[1] = buf[i+1];
                is_tag[2] = buf[i+2];
                if(strcmp(tag, is_tag) == 0)
                {
                    num_tags++;
                }
            }
            close(sockfd);
        }
        else if(n == -1)
        {
            printf("readchunk Error\n");
            close(sockfd);
            return 1;
        }
        //printf("%s", buf);
        bzero(buf, 0);
        n = readchunk(sockfd, buf, len);
    }
    printf("%d\n", num_tags);
    
    close(sockfd);

    return 0;
}


ssize_t readchunk( int sockfd, void *buf, size_t len ) {
    
     
     return recv(sockfd, buf, len, 0);
     
    /*
     * Define recvx to return exactly len bytes unless an error occurs or the socket
     * closes.
     */
}
