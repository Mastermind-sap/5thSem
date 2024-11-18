#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define SHM_NAME "/shm_buffer"
#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int head;
    int tail;
} ring_buffer;

// Declare semaphores globally for cleanup
sem_t *sem_empty, *sem_full, *sem_mutex;
int shm_fd;
ring_buffer *shm_ptr;

// Cleanup function to unlink semaphores and shared memory
void cleanup() {
    printf("Cleaning up...\n");
    sem_close(sem_empty);
    sem_close(sem_full);
    sem_close(sem_mutex);
    sem_unlink("/sem_empty");
    sem_unlink("/sem_full");
    sem_unlink("/sem_mutex");
    shm_unlink(SHM_NAME);
    exit(0);
}

// Signal handler for interrupt signal
void handle_signal(int signal) {
    cleanup();
}

int main() {
    // Set up signal handler
    signal(SIGINT, handle_signal);

    // Create shared memory
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shm_fd, sizeof(ring_buffer)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    shm_ptr = mmap(0, sizeof(ring_buffer), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize shared memory
    shm_ptr->head = 0;
    shm_ptr->tail = 0;

    // Initialize semaphores
    sem_empty = sem_open("/sem_empty", O_CREAT, 0666, BUFFER_SIZE);
    sem_full = sem_open("/sem_full", O_CREAT, 0666, 0);
    sem_mutex = sem_open("/sem_mutex", O_CREAT, 0666, 1);

    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED || sem_mutex == SEM_FAILED) {
        perror("sem_open");
        cleanup();
    }

    // Producer loop: Write integers into the buffer
    int value = 1;
    while (1) {
        sem_wait(sem_empty); // Wait for an empty slot
        sem_wait(sem_mutex); // Lock access to the buffer

        // Write the value into the buffer
        shm_ptr->buffer[shm_ptr->head] = value;
        printf("Produced: %d\n", value);
        fflush(stdout);
        shm_ptr->head = (shm_ptr->head + 1) % BUFFER_SIZE;
        value++;

        sem_post(sem_mutex); // Unlock access
        sem_post(sem_full);  // Signal that a slot is full

        sleep(1); // Simulate some work
    }

    return 0;
}

