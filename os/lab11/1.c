#include <stdio.h>

void worstFit(int blockSize[], int blocks, int fileSize[], int files) {
    int allocation[files];
    int fragment[files];

    // Initialize allocation array
    for (int i = 0; i < files; i++)
        allocation[i] = -1;

    for (int i = 0; i < files; i++) {
        int worstIdx = -1;
        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= fileSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                    worstIdx = j;
            }
        }

        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            fragment[i] = blockSize[worstIdx] - fileSize[i];
            blockSize[worstIdx] -= fileSize[i];
        }
    }

    printf("\nWorst Fit Allocation\nFile No.\tFile Size\tBlock No.\tFragment\n");
    for (int i = 0; i < files; i++) {
        if (allocation[i] != -1)
            printf("%d\t\t%d\t\t%d\t\t%d\n", i + 1, fileSize[i], allocation[i] + 1, fragment[i]);
        else
            printf("%d\t\t%d\t\tNot Allocated\n", i + 1, fileSize[i]);
    }
}

void bestFit(int blockSize[], int blocks, int fileSize[], int files) {
    int allocation[files];
    int fragment[files];

    // Initialize allocation array
    for (int i = 0; i < files; i++)
        allocation[i] = -1;

    for (int i = 0; i < files; i++) {
        int bestIdx = -1;
        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= fileSize[i]) {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                    bestIdx = j;
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            fragment[i] = blockSize[bestIdx] - fileSize[i];
            blockSize[bestIdx] -= fileSize[i];
        }
    }

    printf("\nBest Fit Allocation\nFile No.\tFile Size\tBlock No.\tFragment\n");
    for (int i = 0; i < files; i++) {
        if (allocation[i] != -1)
            printf("%d\t\t%d\t\t%d\t\t%d\n", i + 1, fileSize[i], allocation[i] + 1, fragment[i]);
        else
            printf("%d\t\t%d\t\tNot Allocated\n", i + 1, fileSize[i]);
    }
}

void firstFit(int blockSize[], int blocks, int fileSize[], int files) {
    int allocation[files];
    int fragment[files];

    // Initialize allocation array
    for (int i = 0; i < files; i++)
        allocation[i] = -1;

    for (int i = 0; i < files; i++) {
        for (int j = 0; j < blocks; j++) {
            if (blockSize[j] >= fileSize[i]) {
                allocation[i] = j;
                fragment[i] = blockSize[j] - fileSize[i];
                blockSize[j] -= fileSize[i];
                break;
            }
        }
    }

    printf("\nFirst Fit Allocation\nFile No.\tFile Size\tBlock No.\tFragment\n");
    for (int i = 0; i < files; i++) {
        if (allocation[i] != -1)
            printf("%d\t\t%d\t\t%d\t\t%d\n", i + 1, fileSize[i], allocation[i] + 1, fragment[i]);
        else
            printf("%d\t\t%d\t\tNot Allocated\n", i + 1, fileSize[i]);
    }
}

int main() {
    int blocks, files;

    printf("Enter the number of blocks: ");
    scanf("%d", &blocks);
    printf("Enter the number of files: ");
    scanf("%d", &files);

    int blockSize[blocks], fileSize[files];

    printf("Enter the size of the blocks:\n");
    for (int i = 0; i < blocks; i++) {
        printf("Block %d: ", i + 1);
        scanf("%d", &blockSize[i]);
    }

    printf("Enter the size of the files:\n");
    for (int i = 0; i < files; i++) {
        printf("File %d: ", i + 1);
        scanf("%d", &fileSize[i]);
    }

    // Making copies of blockSize array for each allocation method
    int blockSizeCopy1[blocks], blockSizeCopy2[blocks];
    for (int i = 0; i < blocks; i++) {
        blockSizeCopy1[i] = blockSize[i];
        blockSizeCopy2[i] = blockSize[i];
    }

    worstFit(blockSize, blocks, fileSize, files);

    // Restore original block sizes for next allocation
    for (int i = 0; i < blocks; i++) blockSize[i] = blockSizeCopy1[i];
    bestFit(blockSize, blocks, fileSize, files);

    // Restore original block sizes for next allocation
    for (int i = 0; i < blocks; i++) blockSize[i] = blockSizeCopy2[i];
    firstFit(blockSize, blocks, fileSize, files);

    return 0;
}

