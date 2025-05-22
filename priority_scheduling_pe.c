#include <stdio.h>
#include <limits.h>
#include <string.h>

// Define a structure for Process
struct Process {
    int id;              // Process ID
    int arrivalTime;     // Arrival time of the process
    int burstTime;       // Burst time of the process
    int priority;        // Priority of the process (lower value = higher priority)
    int remainingTime;   // Remaining burst time
    int completionTime;  // Completion time
    int waitingTime;     // Waiting time
    int turnaroundTime;  // Turnaround time
    int isCompleted;     // Flag to check if the process is completed
};

// Preemptive Priority Scheduling simulation that records Gantt Chart data
// ganttOrder[]: records process IDs when a context switch occurs
// ganttTime[]: records the corresponding time for that switch (ganttTime has count+1 elements)
void priorityPreemptive(struct Process p[], int n, int *ganttCount, int ganttOrder[], int ganttTime[]) {
    int currentTime = 0, completed = 0, lastExecuted = -1;
    int index = 0; // Index for Gantt Chart arrays

    while (completed < n) {
        int minPriority = INT_MAX;
        int shortest = -1;

        // Find the process with the highest priority (lowest priority value) among available ones
        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && !p[i].isCompleted && p[i].priority < minPriority) {
                minPriority = p[i].priority;
                shortest = i;
            }
        }

        if (shortest == -1) {
            currentTime++; // No process is ready; move time forward
            continue;
        }

        // Record the time and process only if the process changes (merging consecutive execution)
        if (lastExecuted != p[shortest].id) {
            ganttOrder[index] = p[shortest].id;
            ganttTime[index] = currentTime;
            index++;
        }
        lastExecuted = p[shortest].id;

        // Execute the chosen process for 1 unit of time
        p[shortest].remainingTime--;
        currentTime++;

        // If process finishes, update its completion, turnaround, and waiting times
        if (p[shortest].remainingTime == 0) {
            completed++;
            p[shortest].isCompleted = 1;
            p[shortest].completionTime = currentTime;
            p[shortest].turnaroundTime = p[shortest].completionTime - p[shortest].arrivalTime;
            p[shortest].waitingTime = p[shortest].turnaroundTime - p[shortest].burstTime;
        }
    }
    // Append the final time to the Gantt Chart data
    ganttTime[index] = currentTime;
    *ganttCount = index;
}

// Function to print the Gantt Chart with aligned timeline numbers
void printGanttChartPreemptive(int ganttOrder[], int ganttTime[], int count) {
    // 'count' is the number of segments (ganttTime has count+1 elements)
    int scale = 2; // Scale factor: each unit time corresponds to (scale) characters
    int blockWidth[count];  // Width of each segment in characters
    int cumulative[count+1]; // Cumulative positions of the plus signs
    cumulative[0] = 0;

    // Compute block widths and cumulative positions based on the time differences
    for (int i = 0; i < count; i++) {
        int timeDiff = ganttTime[i+1] - ganttTime[i];
        blockWidth[i] = timeDiff * scale;
        if (blockWidth[i] < 2) {
            blockWidth[i] = 2; // Ensure a minimum width for visibility
        }
        cumulative[i+1] = cumulative[i] + blockWidth[i] + 1; // +1 for the plus sign
    }

    // Print top border
    printf("\nGantt Chart:\n");
    printf("+");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // Print process row (center the label within each block)
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

    // Build and print the timeline string with numbers aligned under plus signs
    int totalWidth = cumulative[count] + 1;
    char timeline[totalWidth + 1];
    for (int i = 0; i < totalWidth; i++) {
        timeline[i] = ' ';
    }
    timeline[totalWidth] = '\0';

    // For each plus sign position (stored in cumulative), place the corresponding time
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

// Function to compute and display process details and averages
void computeResults(struct Process p[], int n) {
    float totalWaitingTime = 0, totalTurnaroundTime = 0;
    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += p[i].waitingTime;
        totalTurnaroundTime += p[i].turnaroundTime;
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].arrivalTime, p[i].burstTime, p[i].priority,
               p[i].completionTime, p[i].waitingTime, p[i].turnaroundTime);
    }
    printf("\nAverage Waiting Time = %.2f\n", totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", totalTurnaroundTime / n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival time, burst time, and priority for process P%d: ", p[i].id);
        scanf("%d %d %d", &p[i].arrivalTime, &p[i].burstTime, &p[i].priority);
        p[i].remainingTime = p[i].burstTime;
        p[i].isCompleted = 0;
    }

    // Gantt Chart arrays for preemptive scheduling
    int ganttOrder[1000], ganttTime[1000];
    int ganttCount = 0;

    // Run the Priority Scheduling (Preemptive) simulation
    priorityPreemptive(p, n, &ganttCount, ganttOrder, ganttTime);

    // Compute and display process details and averages
    computeResults(p, n);

    // Print the aligned Gantt Chart
    printGanttChartPreemptive(ganttOrder, ganttTime, ganttCount);

    return 0;
}
