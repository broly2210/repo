#include <stdio.h>

struct Process {
    int pid;           // Process ID
    int arrival_time;  // Arrival Time
    int burst_time;    // Burst Time
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

// Function to sort processes by arrival time
void sortProcessesByArrival(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                struct Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}

void calculateTimes(struct Process p[], int n) {
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        // Calculate Completion Time
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time; // If CPU is idle until process arrives
        }
        p[i].completion_time = current_time + p[i].burst_time;
        current_time = p[i].completion_time;

        // Calculate Turnaround Time and Waiting Time
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
    }
}

void displayChart(struct Process p[], int n) {
    printf("\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", 
            p[i].pid, p[i].arrival_time, p[i].burst_time, 
            p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
    int TATsum = 0;
    int WTsum = 0;
    for(int i=0;i<n;i++){
        TATsum += p[i].turnaround_time;
        WTsum += p[i].waiting_time;
    }
    
    printf("\n\nAverage Turn around Time :- %d\n",(float)TATsum/n);
    printf("Average Waiting time :- %d\n", (float)WTsum/n);
}

void displayGanttChart(struct Process p[], int n) {
    printf("\nGantt Chart:\n|");
    for (int i = 0; i < n; i++) {
        printf(" P%d |", p[i].pid);
    }
    printf("\n0");
    for (int i = 0; i < n; i++) {
        printf("    %d", p[i].completion_time);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    
    // Input Process Details
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter arrival time and burst time for Process P%d: ", p[i].pid);
        scanf("%d %d", &p[i].arrival_time, &p[i].burst_time);
    }

    // Sort processes by arrival time
    sortProcessesByArrival(p, n);

    // Calculate Completion, Turnaround, and Waiting Times
    calculateTimes(p, n);

    // Display Result Chart and Gantt Chart
    displayChart(p, n);
    displayGanttChart(p, n);

    return 0;
}
