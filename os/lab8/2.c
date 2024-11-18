#include <stdio.h>
#include <string.h>

#define MAX 10

typedef struct {
    char name[10];
    int time;
    int alloc;
} Job;

int jobs, available;
Job job[MAX];

void prevent_deadlock() {
    int finished[MAX] = {0}, count = 0;

    printf("\nSafe sequence: ");
    while (count < jobs) {
        int found = 0;
        for (int i = 0; i < jobs; i++) {
            if (!finished[i] && job[i].alloc <= available) {
                // Allocate resources and mark job as finished
                available += job[i].alloc;
                finished[i] = 1;
                found = 1;
                count++;
                printf("%s ", job[i].name);
            }
        }
        if (!found) {
            printf("\nSystem is in unsafe state. Deadlock could occur!\n");
            return;
        }
    }
    printf("\nSystem is in safe state.\n");
}

int main() {
    printf("Enter the number of jobs: ");
    scanf("%d", &jobs);

    for (int i = 0; i < jobs; i++) {
        printf("Enter name and time for job %d: ", i+1);
        scanf("%s %d", job[i].name, &job[i].time);
        printf("Enter allocated resources for job %s: ", job[i].name);
        scanf("%d", &job[i].alloc);
    }

    printf("Enter the available resources: ");
    scanf("%d", &available);

    prevent_deadlock();

    return 0;
}

