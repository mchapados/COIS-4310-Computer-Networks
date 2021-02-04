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
        int verb; // 1=login, 2=messageAll, 3=privateMessage, 4=who 5=quit
        char data[256];
    };
    int sock; // socket descriptor
    struct sockaddr_in srv; // used by connect()
    struct packet input, output;
    char username[50];
    int logged_in = 0;

    // start login request
    printf("Enter a username => ");
    scanf("%s", username);
    input.verb = 1;

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket call failed");
        exit(1);
    }

    // connect to the server
    srv.sin_family = AF_INET; // use internet address family
    srv.sin_port = htons(53943); // connect to port 1989
    srv.sin_addr.s_addr = inet_addr("192.197.151.116"); // connect to loki
    if (connect(sock, (struct sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("connect call failed");
        exit(1);
    }

    
        strcpy(input.source, username);
        printf("Message: ");
        fgets(input.data, 256, stdin);

        // check for exit condition
        if (strncmp(input.data, "bye", 3) == 0) {
            printf("Sorry to see you go\n");
            exit(0);
        }

        send(sock, &input, 512, 0);
        if (recv(sock, &output, 512, 0) > 0)
            printf("%s: %s\n", output.source, output.data);
        else {
            printf("Server has died\n");
            close(sock);
            exit(1);
        }
    
    


    // // send login request
    // write(sock, &input, 512);

    // // send and receive messages
    // while (read(sock, &output, 512) > 0) {
    //     // print message from server
    //     printf("%s: %s\n", output.source, output.data);
        
    //     input.version = 1;
    //     strcpy(input.source, username);
    //     printf("Send message to: "); // username, all, or bye
    //     scanf("%s", input.destination);

    //     // check for exit condition
    //     if (strncmp(input.destination, "bye", 3) == 0) {
    //         input.verb = 5;
    //         write(sock, &input, 512); // notify server that user has left
    //         printf("Sorry to see you go\n");
    //         exit(0);
    //     }
        
    //     // get message to send
    //     printf("Message: "); 
    //     fgets(input.data, 256, stdin);
    //     if (strncmp(input.destination, "all", 3) == 0)
    //         input.verb = 2; // message to all
    //     else
    //         input.verb = 3; // private message
        
    //     // send to server
    //     write(sock, &input, 512);
    // }
    // printf("Server has died\n");
    // close(sock);
    // exit(1); 
}