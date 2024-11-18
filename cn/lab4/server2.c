#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_response(int client_socket, const char *header, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "%s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "\r\n"
             "%s",
             header, content_type, strlen(body), body);

    send(client_socket, response, strlen(response), 0);
}

void handle_http_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (received < 0) {
        perror("recv failed");
        close(client_socket);
        return;
    }

    buffer[received] = '\0';

    // Parse the HTTP request
    char method[BUFFER_SIZE], path[BUFFER_SIZE], protocol[BUFFER_SIZE];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    printf("Request: %s %s %s\n", method, path, protocol);

    // Handle only GET requests
    if (strcmp(method, "GET") == 0) {
        // Remove the leading slash from the path
        char *file_path = path + 1;

        // Default to index.html if no file is specified
        if (strlen(file_path) == 0) {
            file_path = "index.html";
        }

        // Open the requested file
        int file_fd = open(file_path, O_RDONLY);
        if (file_fd < 0) {
            // File not found, send 404 response
            const char *body = "<html><body><h1>404 Not Found</h1></body></html>";
            send_response(client_socket, "HTTP/1.1 404 Not Found", "text/html", body);
        } else {
            // Read the file content
            char file_buffer[BUFFER_SIZE];
            int bytes_read = read(file_fd, file_buffer, sizeof(file_buffer) - 1);
            file_buffer[bytes_read] = '\0';
            close(file_fd);

            // Send the HTTP response
            send_response(client_socket, "HTTP/1.1 200 OK", "text/html", file_buffer);
        }
    } else {
        // Unsupported method, send 501 Not Implemented response
        const char *body = "<html><body><h1>501 Not Implemented</h1></body></html>";
        send_response(client_socket, "HTTP/1.1 501 Not Implemented", "text/html", body);
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Main loop to accept and handle incoming connections
    while (1) {
        client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        handle_http_request(client_socket);
    }

    close(server_fd);
    return 0;
}
