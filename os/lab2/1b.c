#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a child process

    if (pid == -1) {
        // If fork() returns a negative value, process creation failed
        printf("Process creation unsuccessful.\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
       	wait(NULL);  // Suspend parent process until child completes
        printf("Parent starts\n");

        // Print even numbers from 0 to 10
        for (int i = 0; i <= 10; i += 2) {
            printf("%d\n", i);
        }

        printf("Parent ends\n");
    } else {
        // Child process
        printf("Child starts\n");

        // Print odd numbers from 0 to 10
        for (int i = 1; i <= 10; i += 2) {
            printf("%d\n", i);
        }

        printf("Child ends\n");
    }

    return 0;
}


