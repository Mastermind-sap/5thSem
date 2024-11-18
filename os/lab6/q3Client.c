#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

void *receive_messages(void *socket) {
    int sock = *(int *)socket;
    char buffer[1024];
    int read_size;

    while ((read_size = recv(sock, buffer, 1024, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Message: %s\n", buffer);
    }

    return NULL;
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];
    pthread_t recv_thread;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("\nSocket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address / Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Create thread to receive messages from server
    pthread_create(&recv_thread, NULL, receive_messages, (void *)&sock);

    // Send messages to server
    while (1) {
        fgets(message, 1024, stdin);
        send(sock, message, strlen(message), 0);
    }

    // Close the socket
    close(sock);
    return 0;
}

