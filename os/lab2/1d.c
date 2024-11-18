#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // Check if filename is provided as a command line argument
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat fileStat;

    // Call stat system call on the filename
    if (stat(argv[1], &fileStat) < 0) {
        // If stat returns a negative value, the file does not exist or error occurred
        perror("File does not exist or error occurred");
        return 1;
    }

    // Display file attributes
    printf("File: %s\n", argv[1]);
    printf("User ID: %d\n", fileStat.st_uid);
    printf("Group ID: %d\n", fileStat.st_gid);
    printf("Block size: %ld\n", fileStat.st_blksize);
    printf("Number of blocks: %ld\n", fileStat.st_blocks);
    printf("File size: %ld bytes\n", fileStat.st_size);
    printf("Number of hard links: %ld\n", fileStat.st_nlink);

    // Convert and display time attributes
    printf("Last access time: %s", ctime(&fileStat.st_atime));
    printf("Last modification time: %s", ctime(&fileStat.st_mtime));
    printf("Last status change time: %s", ctime(&fileStat.st_ctime));

    // Display file permissions
    printf("Permissions: ");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    return 0;
}


