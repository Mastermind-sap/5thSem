#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define WINDOW_SIZE 4
#define MAX_SEQ 10

void send_data(int sock, int base, int next_seq_num, int *ack_received) {
    for (int i = base; i < next_seq_num; i++) {
        if (!ack_received[i % MAX_SEQ]) {
            printf("Sending packet %d\n", i);
            char buffer[256];
            sprintf(buffer, "%d", i);
            send(sock, buffer, strlen(buffer), 0);
        }
    }
}

void receive_ack(int sock, int *ack_received, int *base, int next_seq_num) {
    char buffer[256];
    int ack;
    int len = recv(sock, buffer, 256, 0);
    buffer[len] = '\0';
    ack = atoi(buffer);
    printf("Received ACK %d\n", ack);
    if (ack >= *base) {
        for (int i = *base; i <= ack; i++) {
            ack_received[i % MAX_SEQ] = 1;
        }
        *base = ack + 1;
    }
}

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    // Create socket
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
        printf("Connection failed\n");
        return -1;
    }

    int base = 0, next_seq_num = 0;
    int ack_received[MAX_SEQ] = {0};

    while (base < MAX_SEQ) {
        if (next_seq_num < base + WINDOW_SIZE && next_seq_num < MAX_SEQ) {
            send_data(sock, base, next_seq_num + 1, ack_received);
            next_seq_num++;
        }

        receive_ack(sock, ack_received, &base, next_seq_num);
    }

    close(sock);
    return 0;
}

