//
// Created by sbakaj on 10/20/22.
//

#include "udp_client.h"
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<err.h>
#include"parameters.h"

void send_to_client(flow *buffer, int size) {
    int sock;                            // socket descriptor
    int msg_size, i;
    struct sockaddr_in server{}; // address structures of the server and the client
    struct hostent *servent;             // network host entry required by gethostbyname()


    memset(&server, 0, sizeof(server)); // erase the server structure
    server.sin_family = AF_INET;

    // make DNS resolution of the first parameter using gethostbyname()
    if ((servent = gethostbyname(get_address().c_str())) == nullptr) // check the first parameter
        errx(1, "gethostbyname() failed\n");

    // copy the first parameter to the server.sin_addr structure
    memcpy(&server.sin_addr, servent->h_addr, servent->h_length);

    server.sin_port = htons(get_port());        // server port (network byte order)

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)   //create a client socket
        err(1, "socket() failed\n");

    // create a connected UDP socket
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) == -1)
        err(1, "connect() failed");

    i = send(sock, buffer, size, 0);     // send_to_client data to the server
    if (i == -1)                   // check if data was sent correctly
        err(1, "send_to_client() failed");
    else if (i != size)
        err(1, "send_to_client(): buffer written partially");

    close(sock);
}