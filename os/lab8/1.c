#include <stdio.h>

#define MAX 10

int n, m;
int alloc[MAX][MAX], max[MAX][MAX], avail[MAX];
int need[MAX][MAX], safeSeq[MAX], finished[MAX];

void calc_need() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

int check(int i) {
    for (int j = 0; j < m; j++) {
        if (need[i][j] > avail[j]) {
            return 0;
        }
    }
    return 1;
}

void bankers_algo() {
    int count = 0;
    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (!finished[i] && check(i)) {
                for (int j = 0; j < m; j++) {
                    avail[j] += alloc[i][j];
                }
                safeSeq[count++] = i;
                finished[i] = 1;
                found = 1;
            }
        }
        if (!found) {
            printf("System is in unsafe state.\n");
            return;
        }
    }
    printf("System is in safe state.\nSafe sequence: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", safeSeq[i]);
    }
    printf("\n");
}

int main() {
    printf("Enter the number of processes and resources: ");
    scanf("%d %d", &n, &m);

    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    printf("Enter the max matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the available resources:\n");
    for (int i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }

    calc_need();
    bankers_algo();

    return 0;
}

