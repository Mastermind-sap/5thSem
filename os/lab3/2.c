#include <stdio.h>

int main() {
    int n, i, j, temp;
    int burst_time[20], waiting_time[20], turnaround_time[20], process[20];
    int total_waiting_time = 0, total_turnaround_time = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("Process %d: ", i);
        scanf("%d", &burst_time[i]);
        process[i] = i;  // Store process numbers
    }

    // Sort burst times in ascending order (SJF)
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (burst_time[i] > burst_time[j]) {
                temp = burst_time[i];
                burst_time[i] = burst_time[j];
                burst_time[j] = temp;

                temp = process[i];
                process[i] = process[j];
                process[j] = temp;
            }
        }
    }

    waiting_time[0] = 0; // First process waiting time is 0
    turnaround_time[0] = burst_time[0];

    for (i = 1; i < n; i++) {
        waiting_time[i] = waiting_time[i - 1] + burst_time[i - 1];
        turnaround_time[i] = waiting_time[i] + burst_time[i];
    }

    printf("\nProcess\tExecution Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\n", process[i], burst_time[i], waiting_time[i], turnaround_time[i]);
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }

    printf("Average Waiting Time: %.6f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.6f\n", (float)total_turnaround_time / n);

    return 0;
}

