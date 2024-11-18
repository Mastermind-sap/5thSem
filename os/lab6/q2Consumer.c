#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
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

// Cleanup function to close semaphores and unlink shared memory
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

    // Open shared memory
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    shm_ptr = mmap(0, sizeof(ring_buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Open semaphores
    sem_empty = sem_open("/sem_empty", 0);
    sem_full = sem_open("/sem_full", 0);
    sem_mutex = sem_open("/sem_mutex", 0);

    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED || sem_mutex == SEM_FAILED) {
        perror("sem_open");
        cleanup();
    }

    // Consumer loop: Read integers from the buffer
    while (1) {
        sem_wait(sem_full);  // Wait for a full slot
        sem_wait(sem_mutex); // Lock access to the buffer

        // Read the value from the buffer
        int value = shm_ptr->buffer[shm_ptr->tail];
        printf("Consumed: %d\n", value);
        fflush(stdout);
        shm_ptr->tail = (shm_ptr->tail + 1) % BUFFER_SIZE;

        sem_post(sem_mutex); // Unlock access
        sem_post(sem_empty); // Signal that a slot is empty

        sleep(1); // Simulate some work
    }

    return 0;
}

