#include <stdio.h>

int findLRU(int time[], int n) {
    int minimum = time[0], pos = 0;
    for (int i = 1; i < n; i++) {
        if (time[i] < minimum) {
            minimum = time[i];
            pos = i;
        }
    }
    return pos;
}

void lruPageReplacement(int pages[], int n, int capacity) {
    int frames[capacity], time[capacity];
    int counter = 0, pageFaults = 0, pageHits = 0;
    
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
                time[j] = ++counter; // Update the time of access
                break;
            }
        }

        if (!found) { // Page fault occurs
            int pos = -1;
            
            // Check for an empty frame
            for (int j = 0; j < capacity; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
            }

            // If all frames are filled, find the LRU page to replace
            if (pos == -1) {
                pos = findLRU(time, capacity);
            }
            
            frames[pos] = page;
            time[pos] = ++counter; // Update time of access for the replaced page
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

    lruPageReplacement(pages, n, capacity);

    return 0;
}