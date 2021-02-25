/* --------------------------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #2 - CHAT APPLICATION v2.0
    FILENAME: server.c

    WRITTEN BY: S. Chapados - Feb, 2021
    
    PROGRAM DESCRIPTION:
    Runs a chat server using Internet sockets for communication between two clients.
    
    USAGE: ./server &
    PARAMETERS: NONE
   -------------------------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#define PACKET_SIZE 512
#define MAX_USERS 2

// FUNCTIONS -----------------------------------------------------------------------------------
void catcher(int sig);
int ConnectUser();
int GetMessage(int user);
struct packet Login(int user, struct packet in);
void Quit(int user);
void Who(int user);
struct packet Message(int from, struct packet in);
int CheckSum(char msg[], int value);

// GLOBAL VARIABLES ----------------------------------------------------------------------------
int user0, user1; // clients
char usernames[MAX_USERS][25]; // array storing usernames of connected clients
struct packet { // PACKET STRUCTURE
    int number; // packet number
    int version; // 1=Assignment 1, 2=Assignment 2, etc.
    char source[25]; // username sending the message
    char destination[25]; // receiver of message (all or username)
    int verb; // 1=login, 2=messageAll, 3=privateMessage, 4=who, 5=quit, 6=wait, 7=NAK
    int checksum; // used for checking that data field is not corrupt
    char data[256]; // content of the message
};
int sock; // socket descriptor
struct sockaddr_in srv; // used by bind()
struct sockaddr_in cli; // used by accept()
int cli_len = sizeof(cli); // used by accept()

// MAIN ----------------------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    int not_done = 1;
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
        user0 = ConnectUser();
        printf("User 0 has connected. Waiting for second user...\n");
        
        // accept connection from user 1
        user1 = ConnectUser();
        printf("User 1 has connected. Ready to send messages!\n");

        // create child to deal with connection
        if (fork() == 0) {
            while (not_done) {
                if (!GetMessage(0)) // get message from user 0
                    not_done = 0;
                if (!GetMessage(1)) // get message from user 1
                    not_done = 0;
            }
            close(user0);
            close(user1);
        }
        else { // parent process
            close(user0);
            close(user1);
        }
    }
    exit(0);
} // end main

/*  --------------------------------------------------------------------------------------------
    FUNCTION: catcher
    DESCRIPTION: Used in case socket becomes disconnected.
    RETURNS: VOID
*/
void catcher(int sig) {
    signal(SIGPIPE, catcher);
    close(user0);
    close(user1);
    exit(0);
}

/*  --------------------------------------------------------------------------------------------
    FUNCTION: ConnectUser
    DESCRIPTION: Accepts a connection from a client. If successful, returns the value of the
    accept() function.
    RETURNS: int
*/
int ConnectUser() {
    int user = accept(sock, (struct sockaddr*) &cli, &cli_len);
    if (user < 0) {
        perror("accept call failed");
        exit(1);
    }
    return user;
}

/*  --------------------------------------------------------------------------------------------
    FUNCTION: GetMessage
    DESCRIPTION: Used to send and receive messages. Returns false if recv() fails.
    PARAMETERS:
        user int containing the user number to get a message from
    RETURNS: int (bool)
*/
int GetMessage(int user) {
    struct packet input, output; // packet from client, packet to send from server
    int user_socket = (user == 0) ? user0 : user1;
    if (recv(user_socket, &input, PACKET_SIZE, 0) > 0) {
        printf("Received packet %d with verb %d from User: %s\n", input.number, input.verb, usernames[user]);
        switch (input.verb) {
            case 1: // login request
                output = Login(user, input);
                send(user_socket, &output, PACKET_SIZE, 0);
                break;
            case 5: // user has disconnected
                Quit(0);
                break;
            case 4: // who request
                Who(0);
                break;
            case 2: case 3: // regular message
                output = Message(user, input);
                send(user_socket, &output, PACKET_SIZE, 0);
                if (output.verb == 7)
                    GetMessage(user); // loop until packet is successfully received
                break;
            default:
                break;
        } // end switch
    }
    else {
        printf("User %d has stopped sending\n", user);
        return 0;
    }
    return 1;
}


/*  --------------------------------------------------------------------------------------------
    FUNCTION: Login
    DESCRIPTION: Processes login requests. Saves the client's username and returns an ACK 
    packet from the server.
    PARAMETERS:
        user int containing the user number
        in packet containing the message received from the user
    RETURNS: packet
*/
struct packet Login(int user, struct packet in) {
    struct packet out;
    char msg[] = "Welcome ";
    out.version = 2;
    strcpy(usernames[user], in.source); // store username
    strcat(msg, usernames[user]); // create welcome message
    strcpy(out.source, "Server");
    strcpy(out.data, msg);
    out.verb = 6; // wait for next user
    return out;
}

/*  --------------------------------------------------------------------------------------------
    FUNCTION: Quit
    DESCRIPTION: Called when a user has disconnected. Ends the chat session.
    PARAMETERS:
        user int containing the user number who has left
    RETURNS: packet
*/
void Quit(int user) {
    struct packet out;
    out.version = 2;
    char msg[] = usernames[user];
    strcat(msg, " has quit.");
    strcpy(out.source, "Server");
    strcpy(out.data, msg);
    if (user == 0) // notify other user
        send(user1, &out, PACKET_SIZE, 0);
    else
        send(user0, &out, PACKET_SIZE, 0);
    // close sockets
    close(user0);
    close(user1);
    exit(0);
}

/*  --------------------------------------------------------------------------------------------
    FUNCTION: Who
    DESCRIPTION: Processes who requests. Returns packet containing the list of connected users.
    PARAMETERS:
        user int containing the requesting user number
    RETURNS: packet
*/
void Who(int user) {
    struct packet out;
    int i;
    out.version = 2;
    strcpy(out.source, "Server");
    strcpy(out.data, "Connected users: ");
    for (i = 0; i < MAX_USERS; i++) { // get list of usernames
        strcat(out.data, usernames[i]);
        if (i != MAX_USERS-1)
            strcat(out.data, ", ");
    }
    out.verb = 6; // tell user to wait

    // send response
    if (user == 0) 
        send(user0, &out, PACKET_SIZE, 0);
    else
        send(user1, &out, PACKET_SIZE, 0);

    // tell other user to stop waiting
    strcpy(out.data, "Please send a message.\n");
    out.verb = 0;
    if (user == 1) 
        send(user0, &out, PACKET_SIZE, 0);
    else
        send(user1, &out, PACKET_SIZE, 0);
}

/*  --------------------------------------------------------------------------------------------
    FUNCTION: Message
    DESCRIPTION: 
    PARAMETERS:
        from int containing the user number of the sender
        in packet containing the message received from the user
    RETURNS: packet
*/
struct packet Message(int from, struct packet in) {
    int to = (from == 0) ? user1 : user0; // recipient
    struct packet out;
    out.version = 2;
    
    if (CheckSum(in.data, in.checksum) > 0) { // message is not corrupted
        // relay message to other user
        strcpy(out.source, usernames[from]);
        if (in.verb == 3) {
            strcpy(out.data, "(private message) ");
            strcat(out.data, in.data);
        }
        else
            strcpy(out.data, in.data);
        out.verb = in.verb;
        send(to, &out, PACKET_SIZE, 0);
        // tell sender to wait
        strcpy(out.source, "Server");
        strcpy(out.data, "Message sent.");
        out.verb = 6;
    }
    else { // packet was corrupted
        printf("Packet number %d is corrupted.\n", in.number);
        strcpy(out.source, "Server");
        strcpy(out.data, "Message not received. Please try again.");
        out.verb = 7;
    }
    // send ACK or NAK back to client
    return out;
}

/*  --------------------------------------------------------------------------------------------
    FUNCTION: CheckSum
    DESCRIPTION: Used for packet validation. Sums up the ASCII values of characters in a string
    and returns true if the value is correct.
    PARAMETERS:
        msg char[] containing string to check
        value int containing the value that sum should be equal to
    RETURNS: int (bool)
*/
int CheckSum(char msg[], int value) {
    int sum = 0, i;
    for (i = 0; i < strlen(msg); ++i) // calculate the sum 
        sum += (int) msg[i];
    // return true if value is equal to sum
    return (value == sum) ? 1 : 0;
}