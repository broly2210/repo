#include <stdio.h>

// Function to find the optimal page to replace
int findOptimal(int pages[], int frames[], int n, int capacity, int currentIndex) {
    int farthest = currentIndex, pos = -1;

    for (int i = 0; i < capacity; i++) {
        int j;
        for (j = currentIndex; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }
        
        // If page not found in future, replace this page
        //int j;
        if (j == n) {
            return i;
        }
    }

    return (pos == -1) ? 0 : pos;
}

void optimalPageReplacement(int pages[], int n, int capacity) {
    int frames[capacity];
    int pageFaults = 0, pageHits = 0;

    // Initialize frames to -1 indicating they are empty
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
            int pos = -1;

            // Find an empty frame or use the optimal page replacement strategy
            for (int j = 0; j < capacity; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
            }

            // If no empty frame is found, find the optimal page to replace
            if (pos == -1) {
                pos = findOptimal(pages, frames, n, capacity, i + 1);
            }

            frames[pos] = page;
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

    printf("Enter the number of pages: ");
    scanf("%d", &n);
    
    int pages[n];
    printf("Enter the pages: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter the number of frames: ");
    scanf("%d", &capacity);

    optimalPageReplacement(pages, n, capacity);

    return 0;
}