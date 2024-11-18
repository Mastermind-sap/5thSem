#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 100;
    if (fork() == 0) {
        printf("Child x = %d\n", x);
        x = 200;
        printf("Child modified x = %d\n", x);
    } else {
        printf("Parent x = %d\n", x);
        x = 300;
        printf("Parent modified x = %d\n", x);
    }
    return 0;
}

