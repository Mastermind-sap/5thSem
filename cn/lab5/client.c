#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define WINDOW_SIZE 1
#define MAX_SEQ 3

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Creating socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/Address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    int base = 0, next_seq_num = 0;
    int ack_received[MAX_SEQ] = {0};
    char buffer[BUFFER_SIZE];

    while (base < MAX_SEQ) {
        // Send packets in the window
        if (next_seq_num < base + WINDOW_SIZE && next_seq_num < MAX_SEQ) {
            printf("Sending packet %d\n", next_seq_num);
            snprintf(buffer, BUFFER_SIZE, "%d", next_seq_num);
            send(sock, buffer, strlen(buffer), 0);
            next_seq_num++;
        }

        // Receive ACK
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread > 0) {
            int ack_num = atoi(buffer);
            printf("Received ACK for packet %d\n", ack_num);
            if (ack_num >= base) {
                base = ack_num + 1;
            }
        }

        // Handle retransmission if needed
        if (base != next_seq_num) {
            printf("Retransmitting from packet %d\n", base);
            next_seq_num = base;
        }
    }

    close(sock);
    return 0;
}

