#include <stdio.h>
#include <string.h>

// Structure for Process
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

// The Round Robin scheduling simulation:
// Records the execution segments in ganttOrder[] and ganttTime[].
// ganttOrder[] stores the process ID for each segment,
// ganttTime[] stores the start time of each segment. The final time is appended.
void roundRobin(struct Process p[], int n, int quantum, int *ganttCount, int ganttOrder[], int ganttTime[]) {
    int currentTime = 0, completed = 0;
    int index = 0; // Index for Gantt Chart arrays

    while (completed < n) {
        int executed = 0; // Flag to check if any process was executed in this round

        for (int i = 0; i < n; i++) {
            if (p[i].remainingTime > 0 && p[i].arrivalTime <= currentTime) {
                executed = 1;

                // Record the process execution in the Gantt Chart
                ganttOrder[index] = p[i].id;
                ganttTime[index++] = currentTime;

                // Execute process for quantum or until completion
                if (p[i].remainingTime > quantum) {
                    currentTime += quantum;
                    p[i].remainingTime -= quantum;
                } else {
                    currentTime += p[i].remainingTime;
                    p[i].completionTime = currentTime;
                    p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
                    p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
                    p[i].remainingTime = 0;
                    completed++;
                }
            }
        }

        // If no process was executed (i.e. CPU is idle), advance time
        if (!executed) {
            currentTime++;
        }
    }
    // Append the final time to the Gantt Chart data
    ganttTime[index] = currentTime;
    *ganttCount = index;
}

// Function to print the Round Robin Gantt Chart with aligned timeline numbers.
// The width of each block is proportional to the time slice (difference between consecutive times) multiplied by a scale factor.
void printGanttChartRR(int ganttOrder[], int ganttTime[], int count) {
    int scale = 2; // Scale factor: each unit of time corresponds to (scale) characters
    int blockWidth[count];  // Width (in characters) for each segment
    int cumulative[count+1]; // Cumulative positions (in characters) for the plus signs
    cumulative[0] = 0;

    // Calculate block widths based on the time difference between consecutive segments,
    // and build cumulative positions for timeline alignment.
    for (int i = 0; i < count; i++) {
        int timeDiff = ganttTime[i+1] - ganttTime[i];
        blockWidth[i] = timeDiff * scale;
        if (blockWidth[i] < 2) {
            blockWidth[i] = 2; // Ensure a minimum width for visibility
        }
        cumulative[i+1] = cumulative[i] + blockWidth[i] + 1; // +1 for the plus sign
    }

    // Print top border of the Gantt Chart
    printf("\nGantt Chart:\n");
    printf("+");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // Print process row (center the process label within each block)
    printf("|");
    for (int i = 0; i < count; i++) {
        char label[10];
        sprintf(label, "P%d", ganttOrder[i]);
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

    // Print bottom border
    printf("+");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // Build the timeline string so that each timeline number aligns with the corresponding plus sign.
    int totalWidth = cumulative[count] + 1;
    char timeline[totalWidth + 1];
    for (int i = 0; i < totalWidth; i++) {
        timeline[i] = ' ';
    }
    timeline[totalWidth] = '\0';

    for (int i = 0; i <= count; i++) {
        char numStr[10];
        sprintf(numStr, "%d", ganttTime[i]);
        int pos = cumulative[i];
        for (int j = 0; numStr[j] != '\0' && pos + j < totalWidth; j++) {
            timeline[pos + j] = numStr[j];
        }
    }
    printf("%s\n", timeline);
}

// Function to compute and display process details and average times.
void computeResults(struct Process p[], int n) {
    float totalWaitingTime = 0, totalTurnaroundTime = 0;
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += p[i].waitingTime;
        totalTurnaroundTime += p[i].turnaroundTime;
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].arrivalTime, p[i].burstTime,
               p[i].completionTime, p[i].waitingTime, p[i].turnaroundTime);
    }
    printf("\nAverage Waiting Time = %.2f\n", totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", totalTurnaroundTime / n);
}

int main() {
    int n, quantum;
    
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival time and burst time for process P%d: ", p[i].id);
        scanf("%d %d", &p[i].arrivalTime, &p[i].burstTime);
        p[i].remainingTime = p[i].burstTime;
    }
    
    printf("Enter time quantum: ");
    scanf("%d", &quantum);
    
    // Gantt Chart data arrays
    int ganttOrder[1000], ganttTime[1000];
    int ganttCount = 0;
    
    // Run Round Robin scheduling and record Gantt Chart data
    roundRobin(p, n, quantum, &ganttCount, ganttOrder, ganttTime);
    
    // Compute and display process details and averages
    computeResults(p, n);
    
    // Print the aligned Gantt Chart
    printGanttChartRR(ganttOrder, ganttTime, ganttCount);
    
    return 0;
}
