// udp_echo_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    int max_sd, activity;

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
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        // Wait for activity on the socket
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select error");
            break;
        }

        if (FD_ISSET(sockfd, &readfds)) {
            int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
            if (n < 0) {
                perror("recvfrom error");
                continue;
            }

            // Echo the message back to the client
            if (sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
                perror("sendto error");
            }
        }
    }

    close(sockfd);
    return 0;
}


