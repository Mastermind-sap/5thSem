// udp_factorial_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

// Function to compute factorial
unsigned long long factorial(int n) {
    if (n == 0 || n == 1) return 1;
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];
    int number;
    unsigned long long result;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        addr_len = sizeof(client_addr);

        // Receive message from client
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0) {
            perror("recvfrom error");
            continue;
        }
        buffer[n] = '\0'; // Null-terminate the received string

        // Convert the received string to integer
        number = atoi(buffer);
        printf("Received number: %d\n", number);

        // Calculate factorial
        result = factorial(number);

        // Convert result to string
        snprintf(buffer, BUFFER_SIZE, "%llu", result);

        // Send the result back to the client
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
            perror("sendto error");
        }
    }

    close(sockfd);
    return 0;
}


