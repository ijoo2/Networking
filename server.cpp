/*  server.cpp

    This program receives a file from a remote client and sends back an
    acknowledgement after the entire file has been received.

    Two possible options for closing the connection:
        (1) Send a close command along with the acknowledgement and then close.
        (2) Hold the connection open waiting for additional files / commands.
        The "persistent" field in the header indicates the client's preference.

    Written by Inha Joo January 2014 for CS 361
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

    cout <<"Inha Joo. ijoo2. CS450 HW1. server.cpp" << endl;

    int userInput = 1;

    //define variable
    int sockfd;
    int nsockfd;
    int num;
    int sin_size;
    int portNumber = 0;

    struct sockaddr_in addr_local;
    struct sockaddr_in addr_remote;
    char revbuf[LENGTH];



    // User Input

    /* Check for the following from command-line args, or ask the user:

        Port number to listen to.  Default = 54321.
    */
    portNumber = atoi(argv[1]);

    //  Call SOCKET to create a listening socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) ==-1)
    {
        cout << stderr << "ERROR. Failed to obtain socket descriptor." << endl;
        exit(1);

    }

    else
        cout << "[Server] obtaining socket descriptor successfully..." << endl;

    /* Fill the client socket address struct */
    addr_local.sin_family = AF_INET; // Protocol Family
    addr_local.sin_port = htons(portNumber); // Port number
    addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
    bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct


    //  Call BIND to bind the socket to a particular port number
    /* Bind a special Port */
    if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
    {

        cout << stderr << "ERROR. Failed to bind PORT. Errno = " << errno << endl;
        exit(1);
    }
    else
        cout << "[Server] Binded TCP port " << portNumber <<" to addr 127.0.0.1 successfully" << endl;



    //  Call LISTEN to set the socket to listening for new connection requests.
    /* Listen remote connect/calling */
    if(listen(sockfd,SOMAXCONN) == -1)
    {
        cout << stderr << "ERROR. Failed to listen PORT. Errno = " <<errno<<endl;
        exit(1);
    }
    else
        cout << "[Server] Listening the port " <<portNumber <<" successfully" << endl;

    int success = 0;
    while(success == 0)
    {
        sin_size = sizeof(struct sockaddr_in);

        // Call ACCEPT to accept a new incoming connection request.
        // The result will be a new socket, which will be used for all further
        // communications with this client.

        // For HW1 only handle one connection at a time
        /* Wait a connection, and obtain a new socket file despriptor for single connection */
        if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote,(socklen_t*)&sin_size)) == -1)
        {
            cout << "ERROR obtaining socket descriptor" << endl;
            exit(1);
        }
        else
            printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

        /*Receive File from Client */



        // Call RECV to read in one CS450Header struct

        // Then call RECV again to read in the bytes of the incoming file.
        //      If "saveFile" is non-zero, save the file to disk under the name
        //      "filename".  Otherwise just read in the bytes and discard them.
        //      If the total # of bytes exceeds a limit, multiple RECVs are needed.



        char* fr_name = "test.txt";
        FILE *fr = fopen(fr_name, "a");
        if(fr == NULL)
            cout << "File could not be opened on server." << endl;
        else
        {
            bzero(revbuf, LENGTH);
            int fr_block_sz = 0;
            while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0)
            {
                int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
                if(write_sz < fr_block_sz)
                {
                    error("File write failed on server.\n");
                }
                bzero(revbuf, LENGTH);
                if (fr_block_sz == 0 || fr_block_sz != 512)
                {
                    break;
                }
            }
            if(fr_block_sz < 0)
            {
                if (errno == EAGAIN)
                {
                    cout << "recv() timed out" << endl;
                }
                else
                {
                    cout << "recv() failed" << endl;
                    exit(1);
                }
            }
            cout << "Successfully received from CLIENT." << endl;
            fclose(fr);
        }

        close(nsockfd);
        cout << "[SERVER] CONNECTION with CLIENT closed. Server will wait now..." << endl;
        cin >> userInput;
        if (userInput == 0)
        {
            cout << "Exiting..." << endl;
            success = 1;
        }
        while(waitpid(-1, NULL, WNOHANG) > 0);

    }


    // Send back an acknowledgement to the client, indicating the number of
    // bytes received.  Other information may also be included.

    // If "persistent" is zero, then include a close command in the header
    // for the acknowledgement and close the socket.  Go back to ACCEPT to
    // handle additional requests.  Otherwise keep the connection open and
    // read in another Header for another incoming file from this client.


    system("PAUSE");
    return EXIT_SUCCESS;
}
