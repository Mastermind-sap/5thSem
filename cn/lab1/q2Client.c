#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 55555
#define BUFFER_SIZE 1024

int client_socket;
char nickname[BUFFER_SIZE];

// Function to receive messages from the server
void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    int n;

    while ((n = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }

    printf("Connection lost.\n");
    close(client_socket);
    pthread_exit(NULL);
}

// Function to send messages to the server
void *send_messages(void *arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character
        if (strcmp(buffer, ":exit") == 0) {
            close(client_socket);
            exit(0);
        }
        char message[BUFFER_SIZE];
        snprintf(message, sizeof(message), "%s: %s\n", nickname, buffer);
        send(client_socket, message, strlen(message), 0);
    }
}

int main() {
    struct sockaddr_in server_addr;
    pthread_t receive_thread, send_thread;

    // Get nickname
    printf("Choose your nickname: ");
    fgets(nickname, BUFFER_SIZE, stdin);
    nickname[strcspn(nickname, "\n")] = '\0';  // Remove newline character

    // Create and configure the client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send nickname to server
    //send(client_socket, "NICK", 4, 0);
    send(client_socket, nickname, strlen(nickname), 0);

    // Create threads for receiving and sending messages
    pthread_create(&receive_thread, NULL, receive_messages, NULL);
    pthread_create(&send_thread, NULL, send_messages, NULL);

    // Wait for threads to finish
    pthread_join(receive_thread, NULL);
    pthread_join(send_thread, NULL);

    return 0;
}
