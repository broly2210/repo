#include <stdio.h>
#include <stdbool.h>

// Function to print the table with the current state of Allocation, Request, and Available resources
void printTable(int processes, int resources, int allocation[processes][resources], int request[processes][resources], int available[]) {
    printf("\nProcess | Allocation | Available | Request\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < processes; i++) {
        printf("P%d\t| ", i);
        for (int j = 0; j < resources; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\t| ");
        if (i == 0) {
            for (int j = 0; j < resources; j++) {
                printf("%d ", available[j]);
            }
        } else {
            for (int j = 0; j < resources; j++) {
                printf("  ");
            }
        }
        printf("\t| ");
        for (int j = 0; j < resources; j++) {
            printf("%d ", request[i][j]);
        }
        printf("\n");
    }
}

// Function to check if the system is in a safe state and return the safe sequence
bool isSafe(int processes, int resources, int available[], int allocation[processes][resources], int request[processes][resources], int safeSequence[]) {
    bool finished[processes];
    int work[resources];

    // Initialize work (available resources)
    for (int i = 0; i < resources; i++) {
        work[i] = available[i];
    }

    // Initialize finished for all processes
    for (int i = 0; i < processes; i++) {
        finished[i] = false;
    }

    int count = 0;
    while (count < processes) {
        bool found = false;

        for (int p = 0; p < processes; p++) {
            if (!finished[p]) {
                bool possible = true;

                // Check if the current process can be executed
                for (int r = 0; r < resources; r++) {
                    if (request[p][r] > work[r]) {
                        possible = false;
                        break;
                    }
                }

                // If possible, simulate execution of process
                if (possible) {
                    // Add allocated resources to available (work) after process completes
                    for (int r = 0; r < resources; r++) {
                        work[r] += allocation[p][r];
                    }

                    // Mark process as finished and add to safe sequence
                    safeSequence[count++] = p;
                    finished[p] = true;
                    found = true;

                    printf("\nResources after process P%d completes: ", p);
                    for (int r = 0; r < resources; r++) {
                        printf("%d ", work[r]);
                    }
                }
            }
        }

        // If no process could be found that can be safely executed
        if (!found) {
            return false; // System is not in a safe state (Deadlock detected)
        }
    }
    return true; // System is in a safe state
}

int main() {
    int processes, resources;

    printf("Enter the number of processes: ");
    scanf("%d", &processes);

    printf("Enter the number of resource types: ");
    scanf("%d", &resources);

    int maxResources[resources], allocation[processes][resources], request[processes][resources], available[resources], safeSequence[processes];

    // Taking input for the maximum available resources of each type
    printf("Enter the maximum available resources of each type (A B C ...):\n");
    for (int i = 0; i < resources; i++) {
        printf("Resource %c: ", 'A' + i);
        scanf("%d", &maxResources[i]);
    }

    // Taking input for the allocation matrix
    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < processes; i++) {
        printf("Process P%d:\n", i);
        for (int j = 0; j < resources; j++) {
            printf("Resource %c: ", 'A' + j);
            scanf("%d", &allocation[i][j]);
        }
    }

    // Taking input for the request matrix
    printf("Enter the request matrix:\n");
    for (int i = 0; i < processes; i++) {
        printf("Process P%d:\n", i);
        for (int j = 0; j < resources; j++) {
            printf("Resource %c: ", 'A' + j);
            scanf("%d", &request[i][j]);
        }
    }

    // Calculating the available resources by subtracting the sum of allocations from the max available resources
    for (int i = 0; i < resources; i++) {
        available[i] = maxResources[i];
        for (int j = 0; j < processes; j++) {
            available[i] -= allocation[j][i];
        }
    }

    // Printing the current state
    printTable(processes, resources, allocation, request, available);

    // Checking if the system is in a safe state
    if (isSafe(processes, resources, available, allocation, request, safeSequence)) {
        printf("\nSystem is in a safe state.\nSafe sequence is: ");
        for (int i = 0; i < processes; i++) {
            printf("P%d ", safeSequence[i]);
        }
        printf("\n");
    } else {
        printf("\nSystem is not in a safe state. Deadlock detected.\n");
    }

    return 0;
}