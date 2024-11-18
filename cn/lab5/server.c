#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accepting an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    int expected_seq_num = 0;
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            break;
        }

        int seq_num = atoi(buffer);
        printf("Received packet with sequence number: %d\n", seq_num);

        if (seq_num == expected_seq_num) {
            printf("Sending ACK for sequence number: %d\n", seq_num);
            char ack[BUFFER_SIZE];
            snprintf(ack, BUFFER_SIZE, "%d", seq_num);
            send(new_socket, ack, strlen(ack), 0);
            expected_seq_num++;
        } else {
            printf("Out of order packet, expecting sequence number: %d\n", expected_seq_num);
            // Resend the last ACK
            char ack[BUFFER_SIZE];
            snprintf(ack, BUFFER_SIZE, "%d", expected_seq_num - 1);
            send(new_socket, ack, strlen(ack), 0);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

