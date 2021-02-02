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

int main(int argc, const char * argv[]) {
    int sock; // socket descriptor
    struct sockaddr_in srv; // used by connect()
    char c, rc;
    int more_data = 1;

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

    // send and receive information with server
    while (more_data) {
        if (c != '\n') { // ignore enter
            printf("Input a lower case letter (or 0 to stop) => ");
            c = getchar();
        }

        if (c != 0) {
            if (c != '\n') {
                send(sock, &c, 1, 0);
                if (recv(sock, &rc, 1, 0) > 0)
                    printf("%c\n", rc);
                else {
                    printf("Server has died\n");
                    close(sock);
                    exit(1);
                }
            }
        }
        else
            more_data = 0;
    }
    
    return 0;
}