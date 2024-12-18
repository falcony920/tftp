#ifndef FORTUNEDATE_H
#define FORTUNEDATE_H

// Function declarations for fortune and date execution
void send_rrq(int sockfd, struct sockaddr_in *server_addr, const char *filename);

void receive_data_and_ack(int sockfd, struct sockaddr_in *server_addr);

void receive_multiple_data_and_acks(int sockfd, struct sockaddr_in *server_addr);

#endif
