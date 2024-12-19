#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

// Define buffer size and error codes for clarity
#define BUFFER_SIZE 512 // Maximum size of a TFTP packet (excluding headers)
#define MODE_NETASCII "netascii"
#define MODE_OCTET "octet"
#define WRQ_OPCODE 2
#define RRQ_OPCODE 1
#define DATA_OPCODE 3
#define ACK_OPCODE 4

// Sends a Write Request (WRQ) to the TFTP server
void sendWRQ(int *sd, struct addrinfo *res, const char *file)
{
    // Define a buffer for the WRQ request
    char WRQBuf[BUFFER_SIZE] = {0};

    WRQBuf[1] = WRQ_OPCODE; // Set opcode for WRQ

    // Add the filename to the buffer, followed by the null terminator
    sprintf(WRQBuf + 2, "%s", file);
    WRQBuf[strlen(file) + 3] = 0;

    // Add the transfer mode (netascii) to the buffer
    sprintf(WRQBuf + strlen(file) + 4, MODE_NETASCII);

    // Send the WRQ request to the server
    sendto(*sd, WRQBuf, strlen(file) + 12, 0, res->ai_addr, res->ai_addrlen);
}

// Sends a Read Request (RRQ) to the TFTP server
void sendRRQ(int *sd, struct addrinfo *res, const char *file)
{
    char RRQBuf[BUFFER_SIZE] = {0};
    int offset = 2;

    // Set opcode for RRQ (Read Request)
    RRQBuf[0] = 0;
    RRQBuf[1] = RRQ_OPCODE;

    // Add the filename, followed by null terminator
    strcpy(RRQBuf + offset, file);
    offset += strlen(file);
    RRQBuf[offset++] = 0;

    // Add the transfer mode (octet) to the buffer for binary transfer
    strcpy(RRQBuf + offset, MODE_OCTET);
    offset += strlen(MODE_OCTET);
    RRQBuf[offset++] = 0;

    // Send the RRQ request to the server
    sendto(*sd, RRQBuf, offset, 0, res->ai_addr, res->ai_addrlen);
}

// Receives an ACK packet from the TFTP server
int recvACK(int *sd, struct addrinfo *res)
{
    char RecvBuf[BUFFER_SIZE] = {0};
    socklen_t addr_len = res->ai_addrlen;
    int bytes_received = recvfrom(*sd, RecvBuf, BUFFER_SIZE, 0, res->ai_addr, &addr_len);

    // Check if the packet contains a valid ACK (opcode 4)
    return (bytes_received >= 4 && RecvBuf[1] == ACK_OPCODE) ? 0 : -1;
}

// Receives data packets from the TFTP server
int recvData(int *sd, struct sockaddr *server_addr, socklen_t *addr_len, char *buffer)
{
    int bytes_received = recvfrom(*sd, buffer, BUFFER_SIZE, 0, server_addr, addr_len);

    // Check if the packet contains valid data (opcode 3)
    if (bytes_received >= 4 && buffer[1] == DATA_OPCODE)
    {
        return bytes_received; // Return number of bytes received
    }
    return -1; // Return error if the packet is not a valid data packet
}

// Sends a data packet to the TFTP server
void sendData(int *sd, struct addrinfo *res, const char *data, int packet_number)
{
    char SendBuf[BUFFER_SIZE] = {0};

    // Set opcode for DATA (opcode 3)
    SendBuf[1] = DATA_OPCODE;

    // Set block number (MSB and LSB)
    SendBuf[2] = (packet_number >> 8) & 0xFF; // Most significant byte
    SendBuf[3] = packet_number & 0xFF;        // Least significant byte

    // Copy the actual data into the buffer (starting from index 4)
    memcpy(SendBuf + 4, data, BUFFER_SIZE - 4);

    // Send the DATA packet to the server
    sendto(*sd, SendBuf, BUFFER_SIZE, 0, res->ai_addr, res->ai_addrlen);
}

// Sends an ACK packet to the TFTP server
void sendACK(int *sd, struct sockaddr *server_addr, socklen_t addr_len, int block_number)
{
    char ack_buffer[4] = {0};

    // Set opcode for ACK (opcode 4)
    ack_buffer[1] = ACK_OPCODE;

    // Set block number (MSB and LSB)
    ack_buffer[2] = (block_number >> 8) & 0xFF; // MSB
    ack_buffer[3] = block_number & 0xFF;        // LSB

    // Send the ACK packet to the server
    sendto(*sd, ack_buffer, sizeof(ack_buffer), 0, server_addr, addr_len);
}
