#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // Check if directory name is provided as a command-line argument
        printf("Usage: %s <directory_name>\n", argv[0]);
        return 1;
    }

    // Open the directory using opendir system call
    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        // If opendir returns NULL, the directory does not exist or an error occurred
        perror("Unable to open directory");
        return 1;
    }

    struct dirent *entry;

    // Read and display each entry in the directory using readdir
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory using closedir system call
    closedir(dir);

    return 0;
}


