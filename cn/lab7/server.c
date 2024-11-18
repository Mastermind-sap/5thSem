#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int s, conn;
    char buf[1024];
    struct sockaddr_in server, client;
    socklen_t csize = sizeof(client);
    int counter = 0;  // To track the number of received messages

    s = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 1);
    conn = accept(s, (struct sockaddr *)&client, &csize);

    while (1) {
        recv(conn, buf, 1024, 0);
        if (strcmp(buf, "end") == 0) break;
        printf("Received: %s\n", buf);

        counter++;
        // Intentionally skip ack for the second message
        if (counter != 2) {
            send(conn, "ack", 3, 0);
        } else {
            printf("Simulating packet loss: No ack sent for %s\n", buf);
        }
    }

    close(conn);
    close(s);
    return 0;
}

