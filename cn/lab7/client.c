#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int c;
    char buf[1024], ack[4];
    struct sockaddr_in server;
    struct timeval tv;

    c = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(c, (struct sockaddr *)&server, sizeof(server));

    // Set socket timeout
    tv.tv_sec = 2;  // 2 seconds timeout
    tv.tv_usec = 0;
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    for (int i = 0; i < 5; i++) {
        sprintf(buf, "Message %d", i + 1);
        while (1) {
            send(c, buf, strlen(buf) + 1, 0);
            if (recv(c, ack, 3, 0) > 0 && strcmp(ack, "ack") == 0) {
                printf("Ack received for: %s\n", buf);
                break;
            }
            printf("Timeout, resending: %s\n", buf);
        }
    }

    send(c, "end", 4, 0);
    close(c);
    return 0;
}

