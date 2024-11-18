#include <stdio.h>

int main() {
    int i, n, time_quantum, time = 0, remaining;
    int burst_time[10], waiting_time[10], turnaround_time[10], remaining_time[10];
    int total_waiting_time = 0, total_turnaround_time = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time for each process:\n");
    for (i = 0; i < n; i++) {
        printf("Process %d: ", i);
        scanf("%d", &burst_time[i]);
        remaining_time[i] = burst_time[i];
    }

    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    remaining = n;

    while (remaining != 0) {
        for (i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                if (remaining_time[i] <= time_quantum) {
                    time += remaining_time[i];
                    turnaround_time[i] = time;
                    waiting_time[i] = turnaround_time[i] - burst_time[i];
                    remaining_time[i] = 0;
                    remaining--;
                } else {
                    time += time_quantum;
                    remaining_time[i] -= time_quantum;
                }
            }
        }
    }

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\n", i, burst_time[i], waiting_time[i], turnaround_time[i]);
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }

    printf("Average Waiting Time: %.6f\n", (float)total_waiting_time / (float)n);
    printf("Average Turnaround Time: %.6f\n", (float)total_turnaround_time / (float)n);

    return 0;
}
