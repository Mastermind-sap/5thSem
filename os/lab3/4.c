#include <stdio.h>

int main() {
    int n, i, j, temp;
    int burst_time[20], waiting_time[20], turnaround_time[20], process[20], priority[20];
    int total_waiting_time = 0, total_turnaround_time = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter burst time and priority for each process:\n");
    for (i = 0; i < n; i++) {
        printf("Process %d\n", i);
        printf("Burst Time: ");
        scanf("%d", &burst_time[i]);
        printf("Priority: ");
        scanf("%d", &priority[i]);
        process[i] = i;  // Store process numbers
    }

    // Sort processes based on priority (lower number indicates higher priority)
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (priority[i] > priority[j]) {
                // Swap priority
                temp = priority[i];
                priority[i] = priority[j];
                priority[j] = temp;

                // Swap burst time
                temp = burst_time[i];
                burst_time[i] = burst_time[j];
                burst_time[j] = temp;

                // Swap process number
                temp = process[i];
                process[i] = process[j];
                process[j] = temp;
            }
        }
    }

    waiting_time[0] = 0;  // First process waiting time is 0
    turnaround_time[0] = burst_time[0];  // First process turnaround time is its burst time

    // Calculate waiting time and turnaround time
    for (i = 1; i < n; i++) {
        waiting_time[i] = waiting_time[i - 1] + burst_time[i - 1];
        turnaround_time[i] = waiting_time[i] + burst_time[i];
    }

    // Display process details
    printf("\nProcess\tPriority\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t%d\t\t%d\t\t%d\t\t%d\n", process[i], priority[i], burst_time[i], waiting_time[i], turnaround_time[i]);
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
    }

    // Calculate and display averages
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);

    return 0;
}

