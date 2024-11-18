#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SIZE 2

typedef struct {
    int buf[SIZE];
    int in, out;
    sem_t empty, full;
} RingBuf;

void producer(RingBuf *r) {
    for (int i = 0; i < 5; i++) {
        sem_wait(&r->empty);
        r->buf[r->in] = i;
        r->in = (r->in + 1) % SIZE;
        sem_post(&r->full);
        sleep(1);
    }
}

void consumer(RingBuf *r) {
    for (int i = 0; i < 5; i++) {
        sem_wait(&r->full);
        printf("Consumed: %d\n", r->buf[r->out]);
        r->out = (r->out + 1) % SIZE;
        sem_post(&r->empty);
        sleep(1);
    }
}

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(RingBuf), 0666 | IPC_CREAT);
    RingBuf *r = (RingBuf *)shmat(shmid, NULL, 0);
    r->in = r->out = 0;
    sem_init(&r->empty, 1, SIZE);
    sem_init(&r->full, 1, 0);

    if (fork() == 0) {
        consumer(r);
        exit(0);
    } else {
        producer(r);
        wait(NULL);
    }

    sem_destroy(&r->empty);
    sem_destroy(&r->full);
    shmdt(r);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

