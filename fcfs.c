#include <stdio.h>
#include <string.h>

struct Process 
{
    int id;
    int arrivalTime;  
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

void sortByArrivalTime(struct Process p[], int n) 
{
    struct Process temp;
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = 0; j < n - i - 1; j++) 
        {
            if (p[j].arrivalTime > p[j + 1].arrivalTime) 
            {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

void calculateTimes(struct Process p[], int n) {
    int currentTime = 0;
    
    for (int i = 0; i < n; i++) 
    {
        if (currentTime < p[i].arrivalTime) 
        {
            currentTime = p[i].arrivalTime;  // CPU is idle until this process arrives
        }

        p[i].completionTime = currentTime + p[i].burstTime;
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;

        currentTime = p[i].completionTime;
    }
}

void printTable(struct Process p[], int n) 
{
    printf("\nProcess Scheduling Table:\n");
    printf("+-----+--------------+------------+-----------------+----------------+\n");
    printf("| PID | Arrival Time | Burst Time | Turnaround Time |  Waiting Time  |\n");
    printf("+-----+--------------+------------+-----------------+----------------+\n");

    for (int i = 0; i < n; i++) 
    {
        printf("| %-3d | %-12d | %-10d | %-15d | %-14d |\n",
               p[i].id, p[i].arrivalTime, p[i].burstTime,
               p[i].turnaroundTime, p[i].waitingTime);
    }

    printf("+-----+--------------+------------+-----------------+----------------+\n");
}

void printGanttChart(struct Process p[], int n) 
{
    printf("\nGantt Chart:\n");

    int scale = 2;
    int blockWidth[n];
    for (int i = 0; i < n; i++) 
    {
        blockWidth[i] = p[i].burstTime * scale;
        if (blockWidth[i] < 2) 
        {  
            // Ensure a minimum width for visibility.
            blockWidth[i] = 2;
        }
    }

    // --- Print the top border ---
    printf("+");
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    // --- Print the process ID row ---
    printf("|");
    for (int i = 0; i < n; i++) 
    {
        char label[10];
        sprintf(label, "P%d", p[i].id);
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
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < blockWidth[i]; j++)
            printf("-");
        printf("+");
    }
    printf("\n");

    int printedChars = 0;
    char buf[50];

    sprintf(buf, "%d", p[0].arrivalTime);
    printf("%s", buf);
    printedChars += strlen(buf);

    int cumulativePos = 0;
    for (int i = 0; i < n; i++) 
    {
        cumulativePos += blockWidth[i] + 1;
        sprintf(buf, "%d", p[i].completionTime);
        int len = strlen(buf);
        
        int spaces = cumulativePos - printedChars;
        for (int j = 0; j < spaces; j++) 
        {
            printf(" ");
        }
        printf("%s", buf);
        printedChars += spaces + len;
    }
    printf("\n");
}

int main() 
{
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];

    for (int i = 0; i < n; i++) 
    {
        printf("Enter Process ID, Arrival Time, and Burst Time for Process %d: ", i + 1);
        scanf("%d %d %d", &p[i].id, &p[i].arrivalTime, &p[i].burstTime);
    }

    sortByArrivalTime(p, n);
    calculateTimes(p, n);
    printTable(p, n);
    printGanttChart(p, n);

    return 0;
}