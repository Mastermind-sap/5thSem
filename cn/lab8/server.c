#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s, conn, seq = 0, ack_num;
    char buf[1024];
    struct sockaddr_in server, client;
    socklen_t csize = sizeof(client);

    s = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 1);
    conn = accept(s, (struct sockaddr *)&client, &csize);

    while (1) {
        recv(conn, buf, 1024, 0);
        sscanf(buf, "Packet %d", &ack_num);
        if (ack_num == seq) seq++;
        printf("Received: %s\n", buf);
        sprintf(buf, "ack %d", ack_num);
        send(conn, buf, strlen(buf) + 1, 0);
    }

    close(conn);
    close(s);
    return 0;
}

