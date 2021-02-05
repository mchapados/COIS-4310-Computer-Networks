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
#define PACKET_SIZE 512
#define MAX_USERS 2

void catcher(int sig);
int user0, user1; // clients
char usernames[MAX_USERS][50]; // array storing usernames of connected clients

int main(int argc, const char * argv[]) {
    // packet structure
    struct packet {
        int number; // packet number
        int version; // 1=Assignment 1, 2=Assignment 2, etc.
        char source[50]; // username sending the message
        char destination[50]; // receiver of message (all or username)
        int verb; // 1=login, 2=messageAll, 3=privateMessage, 4=who, 5=quit, 6=wait
        char data[256]; // content of the message
    };
    int sock; // socket descriptor
    struct sockaddr_in srv; // used by bind()
    struct sockaddr_in cli; // used by accept()
    int cli_len = sizeof(cli); // used by accept()
    struct packet input, output; // packet from client, packet to send from server
    int not_done = 1;
    int i; // iterator

    signal(SIGPIPE, catcher);
    
    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    // bind socket to a port
    srv.sin_family = AF_INET; // use Internet address family
    srv.sin_port = htons(53943); // bind socket to port 53943
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
        // accept connection from user 0
        user0 = accept(sock, (struct sockaddr*) &cli, &cli_len);
        if (user0 < 0) {
            perror("accept call failed");
            exit(1);
        }
        printf("User 0 has connected. Waiting for second user...\n");
        
        // accept connection from user 1
        user1 = accept(sock, (struct sockaddr*) &cli, &cli_len);
        if (user1 < 0) {
            perror("accept call failed");
            exit(1);
        }
        printf("User 1 has connected. Ready to send messages!\n");

        // create child to deal with connection
        if (fork() == 0) {
            // output.version = 1;
            // strcpy(output.source, "Server");
            // strcpy(output.data, "Please login");
            // output.verb = 0;
            // send(user0, &output, PACKET_SIZE, 0);
            // strcpy(output.data, "Please wait");
            // output.verb = 6;
            // send(user1, &output, PACKET_SIZE, 0);
            while (not_done) {
                // get message from user 0
                if (recv(user0, &input, PACKET_SIZE, 0) > 0) {
                    // login request
                    if (input.verb == 1) { 
                        strcpy(usernames[0], input.source); // store username
                        strcpy(output.source, "Server");
                        strcpy(output.data, "Welcome ");
                        strcat(output.data, usernames[0]);
                        output.verb = 6; // wait for user 1
                        send(user0, &output, PACKET_SIZE, 0);
                    }
                    // user 0 has quit
                    else if (input.verb == 5) { 
                        strcpy(output.source, "Server");
                        strcpy(output.data, usernames[0]);
                        strcat(output.data, " has quit.");
                        send(user1, &output, PACKET_SIZE, 0);
                        break; // end loop
                    }
                    // who request
                    else if (input.verb == 4) {
                        strcpy(output.source, "Server");
                        strcpy(output.data, "Connected users: ");
                        // get list of usernames
                        for (i = 0; i < MAX_USERS; i++) {
                            strcat(output.data, usernames[i]);
                            if (i != MAX_USERS-1)
                                strcat(output.data, ", ");
                        }
                        output.verb = 6;
                        send(user0, &output, PACKET_SIZE, 0);
                        strcpy(output.data, "Please send a message.");
                        output.verb = 0;
                        send(user1, &output, PACKET_SIZE, 0);
                    }
                    // message to all
                    else {
                        // relay message
                        strcpy(output.source, usernames[0]);
                        strcpy(output.data, input.data);
                        output.verb = 2;
                        send(user1, &output, PACKET_SIZE, 0);
                        // tell user 0 to wait
                        strcpy(output.source, "Server");
                        strcpy(output.data, "Message sent.");
                        output.verb = 6;
                        send(user0, &output, PACKET_SIZE, 0);
                    }
                }
                else
                    not_done = 0;
                
                // get message from user 1
                if (recv(user1, &input, PACKET_SIZE, 0) > 0) {
                    // login request
                    if (input.verb == 1) { 
                        strcpy(usernames[1], input.source); // store username
                        strcpy(output.source, "Server");
                        strcpy(output.data, "Welcome ");
                        strcat(output.data, usernames[1]);
                        output.verb = 6; // wait for message from user 1
                        send(user1, &output, PACKET_SIZE, 0);
                        output.verb = 0; // tell user 0 to stop waiting
                        send(user0, &output, PACKET_SIZE, 0);
                    }
                    // user 1 has quit
                    else if (input.verb == 5) { 
                        strcpy(output.source, "Server");
                        strcpy(output.data, usernames[1]);
                        strcat(output.data, " has quit.");
                        send(user0, &output, PACKET_SIZE, 0);
                        break; // end loop
                    }
                    // who request
                    else if (input.verb == 4) {
                        strcpy(output.source, "Server");
                        strcpy(output.data, "Connected users: ");
                        // get list of usernames
                        for (i = 0; i < MAX_USERS; i++) {
                            strcat(output.data, usernames[i]);
                            if (i != MAX_USERS-1)
                                strcat(output.data, ", ");
                        }
                        output.verb = 6;
                        send(user1, &output, PACKET_SIZE, 0);
                        strcpy(output.data, "Please send a message.");
                        output.verb = 0;
                        send(user0, &output, PACKET_SIZE, 0);
                    }
                    // message to all
                    else {
                        // relay message
                        strcpy(output.source, usernames[1]);
                        strcpy(output.data, input.data);
                        output.verb = 2;
                        send(user0, &output, PACKET_SIZE, 0);
                        // tell user 0 to wait
                        strcpy(output.source, "Server");
                        strcpy(output.data, "Message sent.");
                        output.verb = 6;
                        send(user1, &output, PACKET_SIZE, 0);
                    }
                }
                else
                    not_done = 0;
            }
            close(user0);
            close(user1);
        }
        else {
            close(user0);
            close(user1);
        }
    }
    return 0;
}

// in case socket becomes disconnected
void catcher(int sig) {
    signal(SIGPIPE, catcher);
    close(user0);
    close(user1);
    exit(0);
}