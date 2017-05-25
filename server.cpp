#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

void error(const char *msg) {
    std::cerr << msg << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if(argc < 2) error("ERROR, no port provided"); 

    // File descriptors
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    // Contains internet address information
    struct sockaddr_in server_address, client_address;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
	error("ERROR opening socket");
    bzero((char *) &server_address, sizeof(server_address));
    portno = atoi(argv[1]);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &server_address,
		sizeof(server_address)) < 0) 
	error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(client_address);
    newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &clilen);
    if (newsockfd < 0) 
	error("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    return 0;
}
