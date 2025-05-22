#include <stdio.h>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int waiting_time;
};

void calculateTimes(struct Process p[], int n) {
    int completed = 0, current_time = 0, min_index = -1;
    int gantt_chart[100];  // Stores process IDs for each time unit in Gantt chart
    int gantt_size = 0;    // Size of gantt_chart array

    while (completed < n) {
        min_index = -1;
        int min_priority = 9999;

        // Find the highest priority process (lowest priority number) that has arrived and has remaining time
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0 && p[i].priority < min_priority) {
                min_priority = p[i].priority;
                min_index = i;
            }
        }

        if (min_index == -1) {
            // No available process, move to next time unit
            current_time++;
        } else {
            // Execute the selected process for 1 time unit
            p[min_index].remaining_time--;
            gantt_chart[gantt_size++] = p[min_index].pid;  // Record process ID in Gantt chart
            current_time++;

            // If process is completed
            if (p[min_index].remaining_time == 0) {
                completed++;
                p[min_index].completion_time = current_time;
                p[min_index].turnaround_time = p[min_index].completion_time - p[min_index].arrival_time;
                p[min_index].waiting_time = p[min_index].turnaround_time - p[min_index].burst_time;
            }
        }
    }

    // Display Gantt Chart
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_size; i++) printf("------");
    printf("\n|");
    for (int i = 0; i < gantt_size; i++) printf(" P%d |", gantt_chart[i]);
    printf("\n");
    for (int i = 0; i < gantt_size; i++) printf("------");
    printf("\n0");
    for (int i = 1; i <= gantt_size; i++) printf("    %d", i);
    printf("\n");
}

void displayChart(struct Process p[], int n) {
    printf("\nProcess\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter arrival time, burst time, and priority for Process P%d: ", p[i].pid);
        scanf("%d %d %d", &p[i].arrival_time, &p[i].burst_time, &p[i].priority);
        p[i].remaining_time = p[i].burst_time;  // Initialize remaining time
    }

    calculateTimes(p, n);
    displayChart(p, n);

    return 0;
}