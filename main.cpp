#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int reconnect(int *sockfd, int *connfd){

    struct sockaddr_in servaddr, cli;
    // socket create and varification
    close(*sockfd);
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        printf("socket creation failed...\n");
        return -1;
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(*sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        return -1;
    }
    else
        printf("connected to the server..\n");

    return 0;
}

int func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("\t\t To server : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        ssize_t tmp = write(sockfd, buff, sizeof(buff));
        printf("write size: %zd\n",sockfd);

        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
    return 0;
}

int main()
{
    int sockfd, connfd;
    while (1) {
        while (reconnect(&sockfd, &connfd) != 0);

        // function for chat
        if (func(sockfd)==0){
            break;
        }
    }

    // close the socket
    close(sockfd);
}

/*REFERENCES:
 * Main code inspiration: https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 */
