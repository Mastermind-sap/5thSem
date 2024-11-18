#include <stdio.h>

#define MAX 10

typedef struct {
    char instr[10];
    int src1, src2, dest;
} instr;

void simulate(instr ins[], int n) {
    int stalls = 0, cycles = n;

    printf("Schedule:\n");
    for (int i = 0; i < n; i++) {
        printf("Instruction %d: %s %d %d %d\n", i+1, ins[i].instr, ins[i].src1, ins[i].src2, ins[i].dest);
        if (i > 0 && (ins[i].src1 == ins[i-1].dest || ins[i].src2 == ins[i-1].dest)) {
            printf("Stall after instruction %d due to dependency on instruction %d\n", i+1, i);
            stalls++;
            cycles++;
        }
    }

    printf("\nTotal stalls: %d\n", stalls);
    printf("Total cycles: %d\n", cycles);
}

int main() {
    int n;
    instr ins[MAX];

    printf("Enter number of instructions: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter instruction %d (op src1 src2 dest): ", i+1);
        scanf("%s %d %d %d", ins[i].instr, &ins[i].src1, &ins[i].src2, &ins[i].dest);
    }

    simulate(ins, n);

    return 0;
}

