#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        // If there are not enough command-line arguments, stop the program
        printf("Error.\n");
        return 1;
    }

    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        // If fork() returns a negative value, process creation failed
        printf("Process creation unsuccessful.\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        wait(NULL);  // Suspend parent process until child completes
        printf("Child Terminated.\n");
        exit(0);  // Terminate the parent process
    } else {
        // Child process
        printf("Child starts\n");

        // Load the program in the given path into the child process using exec
        if (execvp(argv[1], &argv[1]) < 0) {
            // If exec returns a negative value, an error occurred
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }

        // Terminate the child process (this should never be reached if exec is successful)
        exit(0);
    }

    return 0;
}


