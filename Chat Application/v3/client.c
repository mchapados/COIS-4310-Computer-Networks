/* ----------------------------------------------------------------------------
    COIS 4310H ASSIGNMENT #4 - CHAT APPLICATION v3.0
    FILENAME: client.c

    WRITTEN BY: Morgan Chapados - March, 2021
    
    PROGRAM DESCRIPTION:
    Runs a client, which connects to the server to allow for two-way 
    communication with a second client. 
    
    USAGE: ./client (with server running)
---------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#define PACKET_SIZE 512

// FUNCTIONS ------------------------------------------------------------------
int CheckSum(char msg[], int corruptible);
void encrypt(char msg[]);
void decrypt(char msg[]);

// MAIN -----------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    struct packet { // PACKET STRUCTURE
        int number; // packet number
        int version; // 1=Assignment 1, 2=Assignment 2, etc.
        char source[25]; // username sending the message
        char destination[25]; // receiver of message (all, username, who, or bye)
        int verb; // 1=login, 2=messageAll, 3=privateMessage, 4=who, 5=quit, 6=wait, 7=NAK
        int checksum; // used for checking that data field is not corrupt
        char data[256]; // content of the message
    };
    int sock; // socket descriptor
    struct sockaddr_in srv; // used by connect()
    struct packet input, output, prev; // packet to send, packet from server, input copy
    char username[25]; // username of this client
    int logged_in = 0, packet_count = 0;
    int i; // iterator

    // start login request
    printf("----------------------------------------------------------------------\n"
        " Welcome to the chat application!\n"
        " Please enter a username (max 25 characters) => ");
    fgets(username, 25, stdin);
    for (i = 0; i < 25; i++) { // remove new line from username
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

    // initialize packet header
    input.version = 3;
    strcpy(input.source, username);

    // show instructions for users
    printf("----------------------------------------------------------------------\n"
        "                             INSTRUCTIONS\n"
        "----------------------------------------------------------------------\n"
        " Enter commands in the TO field (commands must be lower case)\n"  
        "   - DISCONNECT: bye\n"
        "   - GET LIST OF CONNECTED USERS: who\n"
        "   - SEND MESSAGE TO ALL USERS: all\n"
        "   - SEND PRIVATE MESSAGE: recipient username\n"
        "----------------------------------------------------------------------\n");

    // send and receive messages
    while (1) {
        // send login request
        if (!logged_in) { 
            input.verb = 1;
            input.number = ++packet_count; // this should be packet #1
            input.checksum = 0;
            printf("Logging in...\n");
            send(sock, &input, PACKET_SIZE, 0);
            logged_in = 1;
        }
        
        // wait for responce from server
        output.verb = 6;
        while (output.verb == 6 || output.verb == 7) {
            if (recv(sock, &output, PACKET_SIZE, 0) > 0) {
                if (output.verb == 2 || output.verb == 3)
                    decrypt(output.data); // decrypt message 
                // output message
                if (output.verb == 3)
                    printf("Private message from %s:\n  %s\n", output.source, output.data);
                else
                    printf("Message from %s:\n  %s\n", output.source, output.data);
                if (output.verb == 6) // wait
                    printf("\nWaiting for messages...\n");
                // NAK - resend last packet
                else if (output.verb == 7) { 
                    printf("Re-sending last message...\n");
                    prev.checksum = CheckSum(prev.data, 0); // not corruptible this time
                    send(sock, &prev, PACKET_SIZE, 0);
                }
            }
            else {
                printf("Server has died\n");
                close(sock);
                exit(1);
            } 
        }

        // get message destination input from user
        printf("Send message to: ");
        fgets(input.destination, 25, stdin);

        // increment packet count and assign to number
        input.number = ++packet_count;

        // check exit condition
        if (strncmp(input.destination, "bye", 3) == 0) {
            printf("Sorry to see you go\n");
            input.verb = 5;
            // notify server that user has quit
            send(sock, &input, PACKET_SIZE, 0); 
            exit(0);
        }

        // determine message type
        if (strncmp(input.destination, "who", 3) == 0) { // who request
            input.verb = 4;
            input.checksum = 0; 
            printf("Sending request to server...\n");
            send(sock, &input, PACKET_SIZE, 0);
        }
        else { // regular message
            input.verb = (strncmp(input.destination, "all", 3) == 0) ? 2 : 3;
            // get message input from user
            printf("Message: ");
            fgets(input.data, 256, stdin);
            encrypt(input.data); // encrypt message
            input.checksum = CheckSum(input.data, 1); // can be corrupted
            printf("Sending message...\n");
            prev = input; // save copy of packet
            send(sock, &input, PACKET_SIZE, 0);
        }
    } // end while (message loop)    
} // end main

/*  ---------------------------------------------------------------------------
    FUNCTION: CheckSum
    DESCRIPTION: Used for packet validation. Sums up the ASCII values of 
    characters in a string and returns the result.
    PARAMETERS:
        msg char[] : string to create checksum from
        corruptible int (bool) : if true, 10% chance of incorrect return value 
    RETURNS: int
---------------------------------------------------------------------------  */
int CheckSum(char msg[], int corruptible) {
    int sum = 0, i;
    if (corruptible) {
        srand(time(0)); // seed for random number
        if ((rand() % 10) == 1) // 10% chance of corrupting the packet
            return 1984;
    }
    for (i = 0; i < strlen(msg); ++i) 
        sum += (int) msg[i];
    return sum;
}

/*  ---------------------------------------------------------------------------
    FUNCTION: encrypt
    DESCRIPTION: Uses a Vigenere cipher to encrypt a message based on a
    randomly-generated 11-character key. The key is stored in a file so that it
    can be read by the receiving client. A new key is created for each message.
    This is obviously not very secure by modern standards, but at least 
    somewhat better than a ROT algorithm.
    Reference: https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher

    PARAMETERS:
        msg char[] : string to be encrypted
---------------------------------------------------------------------------  */
void encrypt(char msg[]) {
    FILE *fp = fopen("key.txt", "w"); // file to write the key to
    char key[11]; // randomly generated key goes here
    int i, j = 0, val; // iterators, ASCII value of encrypted char

    // generate the key
    for (i = 0; i < 11; ++i){
        srand(time(0)+i); // seed for random number
        // generate random char between ASCII 33-126
        key[i] = (char) (rand() % 94) + 33;
    }

    fprintf(fp, "%s", key); // write key to file
    fclose(fp); // close file

    // encrypt each character
    for (i = 0; i < strlen(msg); ++i) {
        // formula ensures encryption to a character between ASCII 33-126
        // so also encrypts punctuation and numbers, not just letters
        // doesn't encrypt out-of-range chars like \n or spaces
        if (msg[i] > 32 && msg[i] < 127) {
            val = (int) msg[i] + key[j];
            while (val > 126) // wrap around
                val -= 94; 
            msg[i] = (char) val;
        }
        ++j;
        if (j >= 11) // reset to beginning of key
            j = 0;
    }
}

/*  ---------------------------------------------------------------------------
    FUNCTION: decrypt
    DESCRIPTION: Opposite of encrypt function. Reads a key from a file and
    uses it to extract the original plain-text message.
    PARAMETERS:
        msg char[] : string to be decrypted
---------------------------------------------------------------------------  */
void decrypt(char msg[]) {
    FILE *fp; // file containing key
    if ((fp = fopen("key.txt", "r")) == NULL) {
        strcpy(msg, "Unable to decrypt");
        return; // in case file doesn't exist for some reason
    }
    char key[11]; // key from file goes here
    int i, j = 0, val; // iterators, ASCII value of decrypted char

    // get key from file
    for (i = 0; i < 11; ++i) {
        if (feof(fp)) // in case file is shorter than expected
            break;
        key[i] = fgetc(fp);
    }
    fclose(fp); // close file

    // decrypt each character
    // this just does the opposite of encryption formula
    for (i = 0; i < strlen(msg); ++i) {
        if (msg[i] > 32 && msg[i] < 127) {
            val = (int) msg[i] - key[j];
            while (val < 33)
                val += 94;
            msg[i] = (char) val;
        }
        ++j;
        if (j >= 11) // reset to beginning of key
            j = 0;
    }
}