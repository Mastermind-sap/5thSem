#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080  // Port to listen on

void respond(int new_socket) {
    char buffer[1024] = {0};
    char *response_404 = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
    char *response_200 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

    // Read the request from the client
    read(new_socket, buffer, 1024);

    // Get the requested file name from the request (Basic parsing)
    char *file_name = strtok(buffer, " ");
    file_name = strtok(NULL, " ");
    file_name++;  // Skip the leading '/'

    // If no file is requested, default to index.html
    if (strlen(file_name) == 0) {
        file_name = "index.html";
    }

    // Open the requested file
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        // If file not found, send 404 response
        write(new_socket, response_404, strlen(response_404));
    } else {
        // Send 200 OK response header
        write(new_socket, response_200, strlen(response_200));

        // Send the content of the file
        char content[1024];
        while (fgets(content, sizeof(content), file) != NULL) {
            write(new_socket, content, strlen(content));
        }
        fclose(file);
    }

    // Close the socket
    close(new_socket);
}

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port %d\n", PORT);

    // Main loop to accept connections
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        // Respond to the client
        respond(new_socket);
    }

    return 0;
}

