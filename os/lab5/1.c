#include <stdio.h>
#include <pthread.h>

#define SIZE 5

int arr[SIZE] = {4, 2, 5, 1, 3};
int sum = 0;

void* sort_array(void* arg) {
    int temp;
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    pthread_exit(0);
}

void* compute_sum(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }
    pthread_exit(0);
}

int main() {
    pthread_t sort_thread, sum_thread;

    pthread_create(&sort_thread, NULL, sort_array, NULL);
    pthread_join(sort_thread, NULL);

    printf("Sorted Array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pthread_create(&sum_thread, NULL, compute_sum, NULL);
    pthread_join(sum_thread, NULL);

    printf("Sum of Array: %d\n", sum);

    return 0;
}

