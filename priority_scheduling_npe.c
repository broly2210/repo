#include <stdio.h>
#include <string.h>
#include <limits.h>

// Structure to store process details
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
};

// Function to sort processes by arrival time, then by priority
void sortProcesses(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrivalTime > p[j + 1].arrivalTime ||
               (p[j].arrivalTime == p[j + 1].arrivalTime && p[j].priority > p[j + 1].priority)) {
                struct Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// Function to calculate completion, waiting, and turnaround times
void calculateTimes(struct Process p[], int n) {
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < p[i].arrivalTime)
            currentTime = p[i].arrivalTime; // CPU idle until process arrives
        
        p[i].completionTime = currentTime + p[i].burstTime;
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
        currentTime = p[i].completionTime;
    }
}

// Revised function to print Gantt Chart with aligned timeline numbers
void printGanttChart(struct Process p[], int n) {
    printf("\nGantt Chart:\n");

    int blockWidth[n], cumulative[n+1];
    cumulative[0] = 0;
    int scale = 2; // Each unit of burst time is represented by 2 characters

    // Compute block widths and cumulative positions of plus signs
    for (int i = 0; i < n; i++) {
        blockWidth[i] = p[i].burstTime * scale;
        cumulative[i+1] = cumulative[i] + blockWidth[i] + 1; // +1 for the plus sign separator
    }

    // Print top border
    printf("+");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // Print process row
    printf("|");
    for (int i = 0; i < n; i++) {
        char label[10];
        sprintf(label, "P%d", p[i].id);
        int labelLen = strlen(label);
        int leftPadding = (blockWidth[i] - labelLen) / 2;
        int rightPadding = blockWidth[i] - labelLen - leftPadding;
        for (int j = 0; j < leftPadding; j++) printf(" ");
        printf("%s", label);
        for (int j = 0; j < rightPadding; j++) printf(" ");
        printf("|");
    }
    printf("\n");

    // Print bottom border
    printf("+");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < blockWidth[i]; j++) printf("-");
        printf("+");
    }
    printf("\n");

    // Create timeline string of sufficient length (cumulative[n]+1 characters)
    int lineLength = cumulative[n] + 1;
    char timeline[lineLength + 1];
    for (int i = 0; i < lineLength; i++) {
        timeline[i] = ' ';
    }
    timeline[lineLength] = '\0';

    // Place timeline numbers at each plus sign position
    for (int i = 0; i <= n; i++) {
        char numStr[10];
        if (i == 0)
            sprintf(numStr, "%d", 0);
        else
            sprintf(numStr, "%d", p[i-1].completionTime);
        int pos = cumulative[i];
        // Place the number left-aligned at position 'pos'
        for (int j = 0; numStr[j] != '\0' && pos + j < lineLength; j++) {
            timeline[pos + j] = numStr[j];
        }
    }
    printf("%s\n", timeline);
}

// Function to find and display average waiting and turnaround times
void findAverageTime(struct Process p[], int n) {
    float totalWaitingTime = 0, totalTurnaroundTime = 0;
    calculateTimes(p, n);

    // Display process details
    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tCompletion Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        totalWaitingTime += p[i].waitingTime;
        totalTurnaroundTime += p[i].turnaroundTime;
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               p[i].id, p[i].arrivalTime, p[i].burstTime, p[i].priority,
               p[i].completionTime, p[i].waitingTime, p[i].turnaroundTime);
    }

    // Display averages
    printf("\nAverage Waiting Time = %.2f\n", totalWaitingTime / n);
    printf("Average Turnaround Time = %.2f\n", totalTurnaroundTime / n);

    // Print Gantt Chart
    printGanttChart(p, n);
}

// Main function
int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        printf("Enter arrival time, burst time, and priority for process P%d: ", p[i].id);
        scanf("%d %d %d", &p[i].arrivalTime, &p[i].burstTime, &p[i].priority);
    }

    // Sort processes by arrival time and then by priority
    sortProcesses(p, n);
    // Compute and display results along with the Gantt Chart
    findAverageTime(p, n);

    return 0;
}
