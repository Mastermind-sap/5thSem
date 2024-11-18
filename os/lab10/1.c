#include <stdio.h>

int main() {
    int frameSize, numPages, clockPtr = 0, faults = 0;

    printf("Enter number of pages: ");
    scanf("%d", &numPages);

    int pages[numPages];
    printf("Enter page reference string: ");
    for (int i = 0; i < numPages; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter frame size: ");
    scanf("%d", &frameSize);

    int frames[frameSize], refBits[frameSize];
    for (int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        refBits[i] = 0;
    }

    for (int i = 0; i < numPages; i++) {
        int page = pages[i], found = 0;

        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == page) {
                refBits[j] = 1;
                found = 1;
                break;
            }
        }

        if (found) {
            printf("Page %d - Hit\n", page);
        } else {
            printf("Page %d - Miss, ", page);
            if (frames[clockPtr] != -1) {
                printf("replacing Page %d with Page %d\n", frames[clockPtr], page);
            } else {
                printf("adding Page %d\n", page);
            }

            while (refBits[clockPtr] == 1) {
                refBits[clockPtr] = 0;
                clockPtr = (clockPtr + 1) % frameSize;
            }

            frames[clockPtr] = page;
            refBits[clockPtr] = 1;
            clockPtr = (clockPtr + 1) % frameSize;
            faults++;
        }

        printf("Frames: ");
        for (int j = 0; j < frameSize; j++) {
            if (frames[j] != -1) printf("%d ", frames[j]);
            else printf("- ");
        }
        printf("\n");
    }

    printf("\nTotal Page Faults: %d\n", faults);
    return 0;
}

