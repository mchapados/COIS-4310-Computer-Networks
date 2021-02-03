/*
  
 COIS 4310H ASSIGNMENT #1
 NAME: client.c

 WRITTEN BY: Sabrina Chapados - Jan-Feb, 2021
 
 PURPOSE: 
 USAGE: ./client
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
    struct sockaddr_in srv; // used by connect()
    struct packet input, output;
    char username[50];
    int logged_in = 0;

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    // connect to the server
    srv.sin_family = AF_INET; // use internet address family
    srv.sin_port = htons(1989); // connect to port 1989
    srv.sin_addr.s_addr = inet_addr("192.197.151.116"); // connect to loki
    if (connect(sock, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("connect call failed");
        exit(1);
    }

    // send and receive messages
    while (1) {
        input.version = 1;
        if (!logged_in) {
            // log user in
            printf("Enter a username => ");
            fgets(username, 50, stdin);
            strcpy(input.source, username);
            input.verb = 1;
            logged_in = 1;
            printf("Logging in...\n");
        }
        else {
            printf("Send message to: ");
            fgets(input.destination, 50, stdin);
            if (strncmp(input.destination, "bye", 3) == 0) { // end loop
                printf("Sorry to see you go\n");
                exit(0);
            }
            printf("Message: ");
            fgets(input.data, 256, stdin);
            if (strncmp(input.destination, "all", 3) == 0)
                input.verb = 2;
            else
                input.verb = 3;
        }
        send(sock, &input, 512, 0);
        if (recv(sock, &output, 512, 0) > 0)
            printf("%s: %s\n", output.source, output.data);
        else {
            printf("Server has died\n");
            close(sock);
            exit(1);
        } 
    }
    return 0;
}