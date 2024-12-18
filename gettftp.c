#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "utils.h"

#define MAX_BUFFER_SIZE 512 // Maximum buffer size for TFTP
#define TFTP_PORT "69"      // Default TFTP port
#define TFTP_MODE "octet"   // Transfer mode

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <server> <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *server = argv[1];
    const char *filename = argv[2];

    printf("Server Address: %s\n", server);
    printf("File Name: %s\n", filename);

    // Resolve server address
    struct sockaddr_in server_addr;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP

    if (getaddrinfo(server, TFTP_PORT, &hints, &res) != 0)
    {
        perror("Error resolving server address");
        return EXIT_FAILURE;
    }

    server_addr = *(struct sockaddr_in *)res->ai_addr;
    freeaddrinfo(res);

    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return EXIT_FAILURE;
    }

    printf("Socket created. Sending RRQ...\n");

    // Send RRQ
    send_rrq(sockfd, &server_addr, filename);

    // Receive data and ACK
    receive_single_data_and_ack(sockfd, &server_addr);

    // Close socket
    close(sockfd);
    printf("File transfer complete.\n");

    return EXIT_SUCCESS;
}
