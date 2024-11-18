#include <stdio.h>
#include <pthread.h>

void* print_message(void* arg) 
{
    char* message = (char*)arg;
    printf("%s\n", message);
    pthread_exit(NULL);
}

int main() 
{
    pthread_t thread;
    char* message = "Hello, Thread!";
    pthread_create(&thread, NULL, print_message, (void*)message);
    pthread_join(thread, NULL);

    printf("Thread has terminated.\n");
    return 0;
}
