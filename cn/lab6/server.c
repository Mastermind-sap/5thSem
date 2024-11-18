#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int is_palindrome(int num) {
    int reversed = 0, original = num, remainder;

    while (num != 0) {
        remainder = num % 10;
        reversed = reversed * 10 + remainder;
        num /= 10;
    }
    return original == reversed;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int number, result;

    // Creating socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error in creating socket\n");
        exit(1);
    }
    // printf("Server socket created successfully.\n");

    // Configuring server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Port number
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Binding socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error in binding\n");
        exit(1);
    }
    // printf("Binding successful.\n");

    // Listening for connections
    if (listen(server_socket, 5) == 0) {
        printf("Listening...\n");
    } else {
        printf("Error in listening\n");
        exit(1);
    }

    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
    if (client_socket < 0) {
        printf("Error in accepting connection\n");
        exit(1);
    }
    printf("Connection accepted from client.\n");

    // Receiving the number from client
    recv(client_socket, &number, sizeof(number), 0);
    printf("Received number : %d\n", number);

    // Checking if number is palindrome
    result = is_palindrome(number);
    
    // Sending the result back to client
    send(client_socket, &result, sizeof(result), 0);
    printf("Result sent to client: %d\n", result);

    close(client_socket);
    close(server_socket);

    return 0;
}

