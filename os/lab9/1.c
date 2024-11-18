#include <stdio.h>

int findOptimal(int seq[], int fr[], int n, int idx, int m) {
    int res = -1, farthest = idx;
    for (int i = 0; i < m; i++) {
        int j;
        for (j = idx; j < n; j++) {
            if (fr[i] == seq[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
        if (j == n) return i;
    }
    return (res == -1) ? 0 : res;
}

int findLRU(int use[], int m) {
    int min = use[0], res = 0;
    for (int i = 1; i < m; i++) {
        if (use[i] < min) {
            min = use[i];
            res = i;
        }
    }
    return res;
}

void printFrames(int fr[], int m) {
    for (int i = 0; i < m; i++) printf("%d ", fr[i]);
    printf("\n");
}

void fifo(int seq[], int n, int m) {
    int fr[m], f = 0, faults = 0;
    for (int i = 0; i < m; i++) fr[i] = -1;
    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < m; j++) if (fr[j] == seq[i]) hit = 1;
        if (!hit) {
            fr[f] = seq[i];
            f = (f + 1) % m;
            faults++;
        }
        printFrames(fr, m);
    }
    printf("FIFO Faults: %d\n", faults);
}

void lru(int seq[], int n, int m) {
    int fr[m], use[m], t = 0, faults = 0;
    for (int i = 0; i < m; i++) fr[i] = -1,use[i]=-1;
    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < m; j++) if (fr[j] == seq[i]) hit = 1, use[j] = t++;
        if (!hit) {
            int idx = findLRU(use, m);
            //printf("%d\n",idx);
	    fr[idx] = seq[i];
            use[idx] = t++;
            faults++;
        }
        printFrames(fr, m);
    }
    printf("LRU Faults: %d\n", faults);
}

void optimal(int seq[], int n, int m) {
    int fr[m], faults = 0;
    for (int i = 0; i < m; i++) fr[i] = -1;
    for (int i = 0; i < n; i++) {
        int hit = 0;
        for (int j = 0; j < m; j++) if (fr[j] == seq[i]) hit = 1;
        if (!hit) {
            int idx = findOptimal(seq, fr, n, i + 1, m);
            //printf("%d\n",idx);
	    fr[idx] = seq[i];
            faults++;
        }
        printFrames(fr, m);
    }
    printf("Optimal Faults: %d\n", faults);
}

int main() {
    //int seq[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    //int n = sizeof(seq) / sizeof(seq[0]);
    //int m = 3;
    int n,m;
    printf("Enter number of page requests: ");
    scanf("%d",&n);
    printf("Enter number of available frames:");
    scanf("%d",&m);
    int seq[n];
    printf("Enter the pages requested in order: ");
    for(int i=0;i<n;i++)scanf("%d",&seq[i]);
    //printf("%d\n%d\n",m,n);
    //for(int i=0;i<n;i++)printf("%d",seq[i]);
    printf("FIFO:\n");
    fifo(seq, n, m);

    printf("LRU:\n");
    lru(seq, n, m);

    printf("Optimal:\n");
    optimal(seq, n, m);

    return 0;
}

