/*
  
 COIS 4310H ASSIGNMENT #1 - CHAT APPLICATION
 FILENAME: client.c

 WRITTEN BY: Morgan Chapados - Jan-Feb, 2021
 
 PURPOSE: Runs a client, which connects to the server to allow for two-way communication with a second client. 
 USAGE: ./client
 PARAMETERS: NONE
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#define PACKET_SIZE 512

int main(int argc, const char * argv[]) {
    // packet structure
    struct packet {
        int number; // packet number
        int version; // 1=Assignment 1, 2=Assignment 2, etc.
        char source[50]; // username sending the message
        char destination[50]; // receiver of message (all, username, who, or bye)
        int verb; // 1=login, 2=messageAll, 3=privateMessage, 4=who, 5=quit, 6=wait
        char data[256]; // content of the message
    };
    int sock; // socket descriptor
    struct sockaddr_in srv; // used by connect()
    struct packet input, output; // packet to send, packet received from server
    char username[50]; // username of this client
    int logged_in = 0;
    int i; // iterator

    // start login request
    printf("Enter a username => ");
    fgets(username, 50, stdin);
    for (i = 0; i < 50; i++) // remove new line from username
    {
        if (username[i] == '\n') {
            username[i] = '\0';
            break;
        }
    }

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    // connect to the server
    srv.sin_family = AF_INET; // use Internet address family
    srv.sin_port = htons(53943); // connect to port 53943
    srv.sin_addr.s_addr = inet_addr("192.197.151.116"); // connect to loki
    if (connect(sock, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("connect call failed");
        exit(1);
    }

    // send and receive messages
    while (1) {
        input.version = 1;
        strcpy(input.source, username);

        // send login request
        if (!logged_in) { 
            input.verb = 1;
            printf("Logging in...\n");
            send(sock, &input, PACKET_SIZE, 0);
            logged_in = 1;
        }
        
        // wait for responce from server
        do {
            if (recv(sock, &output, PACKET_SIZE, 0) > 0) {
                printf("Message from %s:\n  %s\n", output.source, output.data);
                if (output.verb == 6)
                    printf("Waiting for messages...\n");
            }
            else {
                printf("Server has died\n");
                close(sock);
                exit(1);
            } 
        } while (output.verb == 6);

        // get message input
        printf("Send message to: ");
        fgets(input.destination, 50, stdin);

        // check exit condition
        if (strncmp(input.destination, "bye", 3) == 0) {
            printf("Sorry to see you go\n");
            input.verb = 5;
            // notify server that user has quit
            send(sock, &input, PACKET_SIZE, 0); 
            exit(0);
        }

        // determine message type
        if (strncmp(input.destination, "who", 3) == 0) {
            input.verb = 4;
            printf("Sending request to server...\n");
            send(sock, &input, PACKET_SIZE, 0);
        }
        else {
            input.verb = 2;
            printf("Message: ");
            fgets(input.data, 256, stdin);
            printf("Sending message...\n");
            send(sock, &input, PACKET_SIZE, 0);
        }
    }    
}