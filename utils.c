#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 512
#define TFTP_PORT "69"
#define TFTP_MODE "octet"
// TFTP opcodes
#define RRQ_OPCODE 1
#define DATA_OPCODE 3
#define ACK_OPCODE 4

// Function to build and send RRQ (Read Request) packet
void send_rrq(int sockfd, struct sockaddr_in *server_addr, const char *filename)
{
    unsigned char buffer[MAX_BUFFER_SIZE]; // Use unsigned char for TFTP binary packets
    int len = snprintf((char *)buffer, MAX_BUFFER_SIZE, "%c%c%s%c%s%c", 0, RRQ_OPCODE, filename, 0, TFTP_MODE, 0);
    if (len < 0)
    {
        perror("Error building RRQ packet");
        return;
    }

    ssize_t sent = sendto(sockfd, buffer, len, 0, (struct sockaddr *)server_addr, sizeof(*server_addr));
    if (sent == -1)
    {
        perror("Error sending RRQ packet");
        return;
    }
    printf("Read Request (RRQ) sent for file: %s\n", filename);
}

// Function to receive Data (DAT) and its acknowledgment (ACK)
void receive_data_and_ack(int sockfd, struct sockaddr_in *server_addr)
{
    unsigned char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);

    // Receive first Data packet
    int n = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&from_addr, &addr_len);
    if (n < 0)
    {
        perror("Error receiving data packet");
        return;
    }

    // Check if it's a Data packet
    if (buffer[1] == DATA_OPCODE)
    {
        int block_num = (buffer[2] << 8) | buffer[3]; // Extract block number
        printf("Received Data Block: %d\n", block_num);

        // Send ACK for the received Data packet
        unsigned char ack[4] = {0, ACK_OPCODE, buffer[2], buffer[3]}; // Acknowledge Data block
        sendto(sockfd, ack, sizeof(ack), 0, (struct sockaddr *)&from_addr, addr_len);
        printf("Sent ACK for Block: %d\n", block_num);
    }
    else
    {
        fprintf(stderr, "Unexpected packet received: Opcode %d\n", buffer[1]);
    }
}

// Function to handle multiple Data packets
void receive_multiple_data_and_acks(int sockfd, struct sockaddr_in *server_addr)
{
    unsigned char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);

    while (1)
    {
        // Receive Data packet
        int n = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&from_addr, &addr_len);
        if (n < 0)
        {
            perror("Error receiving data packet");
            return;
        }

        // Check if it's a Data packet
        if (buffer[1] == DATA_OPCODE)
        {
            int block_num = (buffer[2] << 8) | buffer[3];
            printf("Received Data Block: %d\n", block_num);

            // Send ACK for the received Data packet
            unsigned char ack[4] = {0, ACK_OPCODE, buffer[2], buffer[3]};
            sendto(sockfd, ack, sizeof(ack), 0, (struct sockaddr *)&from_addr, addr_len);
            printf("Sent ACK for Block: %d\n", block_num);

            if (n < 512) // Last packet condition
            {
                printf("End of file reached (last packet).\n");
                break;
            }
        }
        else
        {
            fprintf(stderr, "Unexpected packet received: Opcode %d\n", buffer[1]);
            break;
        }
    }
}
