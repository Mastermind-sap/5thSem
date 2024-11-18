// udp_echo_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define NUM_MESSAGES 10

long long current_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len;
    long long start_time, end_time;
    long long total_rtt = 0;
    double average_rtt;

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

    // Send and receive NUM_MESSAGES messages
    for (int i = 0; i < NUM_MESSAGES; i++) {
        snprintf(buffer, BUFFER_SIZE, "Message %d", i + 1);
        addr_len = sizeof(server_addr);

        // Record start time
        start_time = current_time_ms();

        // Send message to server
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_len) < 0) {
            perror("sendto error");
            continue;
        }

        // Receive echo from server
        if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len) < 0) {
            perror("recvfrom error");
            continue;
        }

        // Record end time
        end_time = current_time_ms();

        // Calculate RTT
        long long rtt = end_time - start_time;
        total_rtt += rtt;

        printf("Received: %s, RTT: %lld ms\n", buffer, rtt);

        // Sleep for a bit to avoid overwhelming the server
        usleep(500000); // 500ms
    }

    average_rtt = (double)total_rtt / NUM_MESSAGES;
    printf("Average RTT: %.2f ms\n", average_rtt);

    close(sockfd);
    return 0;
}


