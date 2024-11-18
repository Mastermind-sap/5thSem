#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

pthread_mutex_t mutex;
pthread_cond_t cond;
int shared_resource = 0;
int ready = 0;

void* thread_function(void* arg) {
    int id = *((int*)arg);

    pthread_mutex_lock(&mutex);

    while (!ready) {
        pthread_cond_wait(&cond, &mutex);
    }

    shared_resource += id;
    printf("Thread %d accessed the shared resource, updated value: %d\n", id, shared_resource);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, (void*)&thread_ids[i]);
    }

    sleep(1);  // Simulate some work in the main thread

    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

