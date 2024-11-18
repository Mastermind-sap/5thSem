#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(char *message, int sender_socket) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_socket) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    char buffer[1024];
    int read_size;

    // Receive messages from client
    while ((read_size = recv(sock, buffer, 1024, 0)) > 0) {
        buffer[read_size] = '\0';
        printf("Client %d: %s\n", sock, buffer);
        broadcast_message(buffer, sock);
    }

    // Remove client socket
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == sock) {
            client_sockets[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
    close(sock);
    return NULL;
}

int main() {
    int server_fd, new_socket, addrlen;
    struct sockaddr_in address;

    // Initialize client sockets
    memset(client_sockets, 0, sizeof(client_sockets));

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return -1;
    }

    // Define address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    addrlen = sizeof(address);

    // Bind the socket
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Listen for incoming connections
    listen(server_fd, 3);
    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept a new client
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Add new client to the list
        pthread_mutex_lock(&lock);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = new_socket;
                printf("Client %d connected\n", new_socket);
                break;
            }
        }
        pthread_mutex_unlock(&lock);

        // Create a thread to handle the client
        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, (void *)&new_socket);
    }

    return 0;
}

