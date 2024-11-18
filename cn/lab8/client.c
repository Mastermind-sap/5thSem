#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define W 4
#define N 10

int main() {
    int c, ack_num, base = 0, next = 0, acks[N] = {0};
    char buf[1024], ack[1024];
    struct sockaddr_in server;
    struct timeval tv;

    c = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(c, (struct sockaddr *)&server, sizeof(server));

    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));

    while (base < N) {
        while (next < base + W && next < N) {
            sprintf(buf, "Packet %d", next);
            send(c, buf, strlen(buf) + 1, 0);
            printf("Sent: %s\n", buf);
            next++;
        }

        while (recv(c, ack, 1024, 0) > 0) {
            sscanf(ack, "ack %d", &ack_num);
            acks[ack_num] = 1;
            printf("Received: %s\n", ack);
            while (acks[base]) base++;
        }

        for (int i = base; i < next; i++) {
            if (!acks[i]) {
                sprintf(buf, "Packet %d", i);
                send(c, buf, strlen(buf) + 1, 0);
                printf("Resent: %s\n", buf);
            }
        }
    }

    send(c, "end", 4, 0);
    close(c);
    return 0;
}

