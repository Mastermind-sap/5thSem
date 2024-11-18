// udp_factorial_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len;
    ssize_t n;
    unsigned long long factorial;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter a number to calculate its factorial (or -1 to exit): ");
        int number;
        scanf("%d", &number);

        if (number == -1) {
            break;
        }

        // Convert number to string and send to server
        snprintf(buffer, BUFFER_SIZE, "%d", number);
        addr_len = sizeof(server_addr);
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len) < 0) {
            perror("sendto error");
            continue;
        }

        // Receive the result from the server
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
        if (n < 0) {
            perror("recvfrom error");
            continue;
        }
        buffer[n] = '\0'; // Null-terminate the received string

        // Print the result
        factorial = strtoull(buffer, NULL, 10);
        printf("Factorial result: %llu\n", factorial);
    }

    close(sockfd);
    return 0;
}


