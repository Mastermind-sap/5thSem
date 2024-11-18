#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char command[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Get the command from the user
    printf("Enter the command to execute: ");
    fgets(command, BUFFER_SIZE, stdin);
    command[strcspn(command, "\n")] = 0;  // Remove the newline character

    // Send the command to the server
    send(sock, command, strlen(command), 0);

    // Receive the result from the server
    int bytes_read = read(sock, buffer, BUFFER_SIZE);
    buffer[bytes_read] = '\0';
    printf("Command output:\n%s\n", buffer);

    // Close the socket
    close(sock);

    return 0;
}

