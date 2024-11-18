#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 55555
#define BUFFER_SIZE 1024

// Global lists for clients and their nicknames
int clients[100];
char *nicknames[100];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to broadcast messages to all connected clients
void broadcast(char *message) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; ++i) {
        send(clients[i], message, strlen(message), 0);
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to handle communication with a client
void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE];
    int n;

    while ((n = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[n] = '\0';
        broadcast(buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }

    // Remove client and clean up
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; ++i) {
        if (clients[i] == client_socket) {
            close(client_socket);
            // Broadcast that client has left
            char msg[BUFFER_SIZE];
            snprintf(msg, sizeof(msg), "%s left!\n", nicknames[i]);
            broadcast(msg);
            free(nicknames[i]);
            for (int j = i; j < client_count - 1; ++j) {
                clients[j] = clients[j + 1];
                nicknames[j] = nicknames[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t tid;

    // Create and configure the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Ask for nickname
        //send(client_socket, "NICK", 4, 0);
        char nickname[BUFFER_SIZE];
        recv(client_socket, nickname, BUFFER_SIZE, 0);

        pthread_mutex_lock(&clients_mutex);
        clients[client_count] = client_socket;
        nicknames[client_count] = strdup(nickname);
        client_count++;
        pthread_mutex_unlock(&clients_mutex);

        // Broadcast that a new client has joined
        char msg[BUFFER_SIZE];
        snprintf(msg, sizeof(msg), "%s joined!\n", nickname);
        broadcast(msg);

        // Start a new thread to handle this client
        pthread_create(&tid, NULL, handle_client, (void *)&client_socket);
        pthread_detach(tid);
    }

    close(server_socket);
    return 0;
}
