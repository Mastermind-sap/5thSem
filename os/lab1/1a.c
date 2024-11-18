#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int x = 100;  // Declare a variable x

    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        // If fork() returns a negative value, process creation failed
        printf("Process creation unsuccessful.\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // If fork() returns 0, we are in the child process
        printf("Child process\n");
        printf("Child Process ID: %d\n", getpid());
        printf("Value of x: %d\n", x);
        printf("Parent Process ID: %d\n", getppid());
    } else {
        // If fork() returns a positive value, we are in the parent process
	printf("Parent process\n");
        printf("Parent Process ID: %d\n", getpid());
        printf("Value of x: %d\n", x);
        printf("Shell Process ID: %d\n", getppid());
    }

    return 0;
}

