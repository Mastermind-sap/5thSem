#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define FILENAME "file.txt"
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the network address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("FTP Server is waiting for a connection...\n");

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connection established.\n");

    // Open the file
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("File open failed");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Read the file and send its content to the client
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        send(new_socket, buffer, strlen(buffer), 0);
    }

    printf("File content sent to the client.\n");

    // Close the file and socket
    fclose(file);
    close(new_socket);
    close(server_fd);

    return 0;
}

