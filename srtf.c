#include <stdio.h>
#include <limits.h>
#include <string.h>

struct Process {
    int id;             
    int arrivalTime;
    int burstTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

// Function to print the proportional Gantt Chart
void printGanttChart(int executedProcesses[], int executedTimes[], int executedCount) {
    printf("\nGantt Chart:\n");

    int scale = 2;
    int blockWidth[executedCount];

    for (int i = 0; i < executedCount; i++) {
        blockWidth[i] = scale;
    }

    // --- Print the top border ---
    printf("+");
    for (int i = 0; i < executedCount; i++) {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // --- Print the process ID row ---
    printf("|");
    for (int i = 0; i < executedCount; i++) {
        char label[10];
        sprintf(label, "P%d", executedProcesses[i]);
        int labelLen = strlen(label);
        int leftPadding = (blockWidth[i] - labelLen) / 2;
        int rightPadding = blockWidth[i] - labelLen - leftPadding;
        for (int j = 0; j < leftPadding; j++)
            printf(" ");
        printf("%s", label);
        for (int j = 0; j < rightPadding; j++)
            printf(" ");
        printf("|");
    }
    printf("\n");

    // --- Print the bottom border ---
    printf("+");
    for (int i = 0; i < executedCount; i++) {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // --- Print the timeline ---
    int printedChars = 0;
    char buf[50];

    sprintf(buf, "%d", 0);
    printf("%s", buf);
    printedChars += strlen(buf);

    int cumulativePos = 0;
    for (int i = 0; i < executedCount; i++) {
        cumulativePos += blockWidth[i] + 1;
        sprintf(buf, "%d", executedTimes[i]);
        int len = strlen(buf);

        int spaces = cumulativePos - printedChars;
        for (int j = 0; j < spaces; j++) {
            printf(" ");
        }
        printf("%s", buf);
        printedChars += spaces + len;
    }
    printf("\n");
}

// Shortest Remaining Time First (SRTF) Scheduling
void srtf(struct Process p[], int n) {
    int currentTime = 0;   
    int completed = 0;     
    int minBurstTime;
    int shortest = -1;    
    int remainingBurstTime[n];

    // Arrays to store Gantt Chart information
    int executedProcesses[1000], executedTimes[1000], executedCount = 0;

    for (int i = 0; i < n; i++) {
        remainingBurstTime[i] = p[i].burstTime;
    }

    while (completed < n) {
        minBurstTime = INT_MAX;
        shortest = -1;

        // Find the process with the shortest remaining time
        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && remainingBurstTime[i] > 0 && remainingBurstTime[i] < minBurstTime) {
                minBurstTime = remainingBurstTime[i];
                shortest = i;
            }
        }

        // If no process is available, move time forward
        if (shortest == -1) {
            currentTime++;
            continue;
        }

        // Execute the shortest process for 1 unit of time
        remainingBurstTime[shortest]--;
        currentTime++;

        // Store execution order for Gantt Chart
        executedProcesses[executedCount] = p[shortest].id;
        executedTimes[executedCount] = currentTime;
        executedCount++;

        // If the process is completed
        if (remainingBurstTime[shortest] == 0) {
            completed++;
            p[shortest].completionTime = currentTime;
            p[shortest].turnaroundTime = p[shortest].completionTime - p[shortest].arrivalTime;
            p[shortest].waitingTime = p[shortest].turnaroundTime - p[shortest].burstTime;
        }
    }

    // Print process details
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    float totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
               p[i].id, p[i].arrivalTime, p[i].burstTime, p[i].completionTime, 
               p[i].waitingTime, p[i].turnaroundTime);
        totalWT += p[i].waitingTime;
        totalTAT += p[i].turnaroundTime;
    }

    // Print Average Waiting Time & Turnaround Time
    printf("\nAverage Waiting Time: %.2f", totalWT / n);
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);

    // Print the Gantt Chart
    printGanttChart(executedProcesses, executedTimes, executedCount);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival time and burst time for process P%d: ", p[i].id);
        scanf("%d %d", &p[i].arrivalTime, &p[i].burstTime);
    }

    srtf(p, n);
    return 0;
}
