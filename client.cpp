/* client.cpp

    This program transfers a file to a remote server, possibly through an
    intermediary relay.  The server will respond with an acknowledgement.

    This program then calculates the round-trip time from the start of
    transmission to receipt of the acknowledgement, and reports that along
    with the average overall transmission rate.

    Written by Inha Joo for CS 450 HW1 January 2014
*/



#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netdb.h>


#include <cstdlib>
#include <iostream>

#define LENGTH 512

using namespace std;


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    cout <<"Inha Joo ijoo2. CS450 HW1. client.cpp" << endl;


    /* Variable Definition */
    int sockfd;
    int nsockfd;
    char revbuf[LENGTH];
    int globalPort = 0;


    globalPort = atoi(argv[2]);
    struct sockaddr_in remote_addr;


    /* Get the Socket file descriptor */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout << stderr << "ERROR. failed to obtain socket descriptor" << endl;
        exit(1);
    }

    /* Fill the socket address struct */
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(globalPort);
    inet_pton(AF_INET, argv[1], &remote_addr.sin_addr);
    bzero(&(remote_addr.sin_zero), 8);

    /* Try to connect the remote */
    if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
    {
        cout << stderr << "ERROR.failed to connect to host" << endl;
        exit(1);
    }
    else
        printf("[Client] Connected to server at port %d...ok!\n", globalPort);

    /* Send File to Server */

    char* fs_name = "test.txt";
    char sdbuf[LENGTH];
    printf("[Client] Sending %s to the Server... ", fs_name);
    FILE *fs = fopen(fs_name, "r");
    if(fs == NULL)
    {
        cout << "ERROR: File not found.." << endl;
        exit(1);
    }

    bzero(sdbuf, LENGTH);
    int fs_block_sz;
    while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
    {
        if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
        {
            cout << stderr << "ERROR Failed to send file to " << fs_name << endl;
            break;
        }
        bzero(sdbuf, LENGTH);
    }
    cout << "File " << fs_name << " succesfully sent from CLIENT." << endl;
    //}
    close(sockfd);
    cout << "[CLIENT] Connection lost." << endl;
    return (0);
}

// User Input

/* Check for the following from command-line args, or ask the user:

        Destination ( server ) name and port number
        Relay name and port number.  If none, communicate directly with server
        File to transfer.  Use OPEN(2) to verify that the file can be opened
        for reading, and ask again if necessary.
    */

// Use FSTAT and MMAP to map the file to a memory buffer.  That will let the
// virtual memory system page it in as needed instead of reading it byte
// by byte.

// Open a Connection to the server ( or relay )  TCP for the first HW
// call SOCKET and CONNECT and verify that the connection opened.

// Note the time before beginning transmission

// Create a CS450Header object, fill in the fields, and use SEND to write
// it to the socket.

// Use SEND to send the data file.  If it is too big to send in one gulp
// Then multiple SEND commands may be necessary.

// Use RECV to read in a CS450Header from the server, which should contain
// some acknowledgement information.

// Calculate the round-trip time and
// bandwidth, and report them to the screen along with the size of the file
// and output echo of all user input data.

// When the job is done, either the client or the server must transmit a
// CS450Header containing the command code to close the connection, and
// having transmitted it, close their end of the socket.  If a relay is
// involved, it will transmit the close command to the other side before
// closing its connections.  Whichever process receives the close command
// should close their end and also all open data files at that time.

// If it is desired to transmit another file, possibly using a different
// destination, protocol, relay, and/or other parameters, get the
// necessary input from the user and try again.

// When done, report overall statistics and exit.
