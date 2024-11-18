#include <stdio.h>
#include <limits.h>

int main() {
    int n, completed = 0, time = 0, shortest = 0, finish_time;
    float total_wt = 0, total_tat = 0;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    float arrival[n], burst[n], rt[n], tat[n], wt[n];
    int completed_processes[n];
    
    for(int i = 0; i < n; i++) {
        completed_processes[i] = 0;
    }

    printf("Enter Arrival time and Burst time for each process:\n");
    for(int i = 0; i < n; i++) {
        printf("Process %d:\n", i+1);
        printf("Arrival time: ");
        scanf("%f", &arrival[i]);
        printf("Burst time: ");
        scanf("%f", &burst[i]);
        rt[i] = burst[i];
    }

    while (completed != n) {
        int min_rt = INT_MAX;
        shortest = -1;

        for (int i = 0; i < n; i++) {
            if (arrival[i] <= time && rt[i] < min_rt && rt[i] > 0) {
                min_rt = rt[i];
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        rt[shortest]--;
        time++;

        if (rt[shortest] == 0) {
            completed++;
            finish_time = time;
            tat[shortest] = finish_time - arrival[shortest];
            wt[shortest] = tat[shortest] - burst[shortest];
            total_wt += wt[shortest];
            total_tat += tat[shortest];
        }
    }

    printf("\nP\tAT\tBT\tTAT\tWT\n");
    for(int i = 0; i < n; i++) {
        printf("P%d\t%.1f\t%.1f\t%.1f\t%.1f\n", i+1, arrival[i], burst[i], tat[i], wt[i]);
    }

    printf("\nAverage Waiting Time: %.2f", total_wt / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);

    return 0;
}

