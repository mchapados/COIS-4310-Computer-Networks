/*
  
 COIS 4310H ASSIGNMENT #1
 NAME: server.c

 WRITTEN BY: Sabrina Chapados - Jan-Feb, 2021
 
 PURPOSE: 
 USAGE: ./server &
 PARAMETERS: NONE
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ctype.h>

void catcher(int sig);
int newfd;

int main(int argc, const char * argv[]) {
    int sock; // socket descriptor
    struct sockaddr_in srv; // used by bind()
    struct sockaddr_in cli; // used by accept()
    int cli_len = sizeof(cli); // used by accept()
    int done = 0;
    char c;

    signal(SIGPIPE, catcher);
    
    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    // bind socket to a port
    srv.sin_family = AF_INET; // use Internet addr family
    srv.sin_port = htons(1989); // bind socket to port 1989
    srv.sin_addr.s_addr = htonl(INADDR_ANY); // client can connect to any address
    if (bind(sock, (struct sockaddr *)&srv, sizeof(srv)) < 0) {
        perror("bind call failed");
        exit(1);
    }

    // listen on the socket
    if (listen(sock, 5) < 0) {
        perror("listen call failed");
        exit(1);
    }

    // loop looking for messages
    while (!done) {
        // accept connection
        newfd = accept(sock, (struct sockaddr*) &cli, &cli_len);
        if (newfd < 0) {
            perror("accept call failed");
            done = 1;
        }
        // create child to deal with connection
        if (fork() == 0) {
            while (recv(newfd, &c, 1, 0) > 0) {
                c = toupper(c);
                send(newfd, &c, 1, 0);
            }
            // when client stops sending, close socket and child
            close(newfd);
            exit(0);
        }
        else
            close(newfd);
    }

    return 0;
}

// in case socket becomes disconnected
void catcher(int sig) {
    signal(SIGPIPE, catcher);
    close(newfd);
    exit(0);
}