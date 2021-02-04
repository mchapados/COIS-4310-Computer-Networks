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
#include <string.h>

void catcher(int sig);
int user1, user2;

int main(int argc, const char * argv[]) {
    // packet structure
    struct packet {
        int number;
        int version;
        char source[50];
        char destination[50];
        int verb; // 1=login, 2=messageAll, 3=privateMessage, 4=who 
        char data[256];
    };
    int sock; // socket descriptor
    struct sockaddr_in srv; // used by bind()
    struct sockaddr_in cli; // used by accept()
    int cli_len = sizeof(cli); // used by accept()
    struct packet input, output;
    int not_done = 1;

    signal(SIGPIPE, catcher);
    
    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    // bind socket to a port
    srv.sin_family = AF_INET; // use Internet addr family
    srv.sin_port = htons(53943); // bind socket to port 1989
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
    while (1) {
        // accept connection from user 1
        user1 = accept(sock, (struct sockaddr*) &cli, &cli_len);
        if (user1 < 0) {
            perror("accept call failed");
            exit(1);
        }
        //printf("User 1 has connected. Waiting for second user...\n");
        
        // accept connection from user 2
        user2 = accept(sock, (struct sockaddr*) &cli, &cli_len);
        if (user2 < 0) {
            perror("accept call failed");
            exit(1);
        }
        // printf("User 2 has connected. Ready to send messages!\n");

        // create child to deal with connection
        if (fork() == 0) {
            while (not_done) {
                if (recv(user1, &input, 512, 0) > 0) 
                    send(user2, &input, 512, 0);
                else
                    not_done = 0;
                if (recv(user2, &input, 512, 0) > 0) 
                    send(user1, &input, 512, 0);
                else
                    not_done = 0;
            }
            close(user1);
            close(user2);
        }
        else {
            close(user1);
            close(user2);
        }
    }
    return 0;
}

// in case socket becomes disconnected
void catcher(int sig) {
    signal(SIGPIPE, catcher);
    close(user1);
    close(user2);
    exit(0);
}