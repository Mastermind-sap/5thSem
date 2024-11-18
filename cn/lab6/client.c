#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    int number, result;

    // Creating socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error in creating socket\n");
        exit(1);
    }
    //printf("Client socket created successfully.\n");

    // Configuring server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Port number
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connecting to server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error in connecting to server\n");
        exit(1);
    }
    printf("Connected to server.\n");

    // Input the number from user
    printf("Enter a number to check if palindrome: ");
    scanf("%d", &number);

    // Sending the number to the server
    send(client_socket, &number, sizeof(number), 0);

    // Receiving the result from the server
    recv(client_socket, &result, sizeof(result), 0);

    // Displaying the result
    if (result) {
        printf("The number %d is a palindrome.\n", number);
    } else {
        printf("The number %d is not a palindrome.\n", number);
    }

    close(client_socket);

    return 0;
}

