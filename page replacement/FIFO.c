#include <stdio.h>

void fifoPageReplacement(int pages[], int n, int capacity) {
    int frames[capacity];
    int front = 0, pageFaults = 0, pageHits = 0;
    
    // Initialize all frames to -1 (indicating they are empty)
    for (int i = 0; i < capacity; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int found = 0;
        
        // Check if the page is already in one of the frames
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == page) {
                found = 1;
                pageHits++; // Increment page hits if page is already in a frame
                break;
            }
        }

        if (!found) { // Page fault occurs
            frames[front] = page;
            front = (front + 1) % capacity; // FIFO order replacement
            pageFaults++;
        }

        // Print current frame state
        printf("Frames: ");
        for (int j = 0; j < capacity; j++) {
            if (frames[j] == -1) {
                printf("- ");
            } else {
                printf("%d ", frames[j]);
            }
        }
        printf("\n");
    }
    
    printf("Total Page Faults: %d\n", pageFaults);
    printf("Total Page Hits: %d\n", pageHits);
}

int main() {
    int n, capacity;

    printf("Enter the length of reference string: ");
    scanf("%d", &n);
    
    int pages[n];
    printf("Enter the reference string : ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter the number of frames: ");
    scanf("%d", &capacity);

    fifoPageReplacement(pages, n, capacity);

    return 0;
}