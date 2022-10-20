//
// Created by sbakaj on 10/20/22.
//

#include "udp_client.h"
#include<cstdio>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<err.h>
#include"parameters.h"

#define BUFFER 1024                // buffer length

void send() {
    int sock;                             // socket descriptor
    int msg_size, i;
    struct sockaddr_in server{}, from{}; // address structures of the server and the client
    struct hostent *servent;            // network host entry required by gethostbyname()
    socklen_t len, fromlen;
    char buffer[BUFFER];

    memset(&server,0,sizeof(server)); // erase the server structure
    server.sin_family = AF_INET;

    // make DNS resolution of the first parameter using gethostbyname()
    if ((servent = gethostbyname(get_address().c_str())) == nullptr) // check the first parameter
        errx(1,"gethostbyname() failed\n");

    // copy the first parameter to the server.sin_addr structure
    memcpy(&server.sin_addr,servent->h_addr,servent->h_length);

    server.sin_port = htons(get_port());        // server port (network byte order)

    if ((sock = socket(AF_INET , SOCK_DGRAM , 0)) == -1)   //create a client socket
        err(1,"socket() failed\n");

    printf("* Server socket created\n");

    len = sizeof(server);
    fromlen = sizeof(from);

    printf("* Creating a connected UDP socket using connect()\n");
    // create a connected UDP socket
    if (connect(sock, (struct sockaddr *)&server, sizeof(server))  == -1)
        err(1, "connect() failed");

    //send data to the server
    while((msg_size=read(STDIN_FILENO,buffer,BUFFER)) > 0)
        // read input data from STDIN (console) until end-of-line (Enter) is pressed
        // when end-of-file (CTRL-D) is received, n == 0
    {
        i = send(sock,buffer,msg_size,0);     // send data to the server
        if (i == -1)                   // check if data was sent correctly
            err(1,"send() failed");
        else if (i != msg_size)
            err(1,"send(): buffer written partially");

        // obtain the local IP address and port using getsockname()
        if (getsockname(sock,(struct sockaddr *) &from, &len) == -1)
            err(1,"getsockname() failed");
    }
    close(sock);
}