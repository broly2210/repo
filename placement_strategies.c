#include <stdio.h>

int p_count, b_count;
int blocks[10], processes[10], alloc[10], used[10];

void first_fit() {
    for (int i = 0; i < p_count; i++) alloc[i] = -1;
    for (int j = 0; j < b_count; j++) used[j] = 0;
    for (int i = 0; i < p_count; i++)
        for (int j = 0; j < b_count; j++)
            if (!used[j] && blocks[j] >= processes[i]) {
                alloc[i] = j;
                used[j] = 1;
                break;
            }
}

void next_fit() {
    for (int i = 0; i < p_count; i++) alloc[i] = -1;
    for (int j = 0; j < b_count; j++) used[j] = 0;
    int last = 0;
    for (int i = 0; i < p_count; i++) {
        for (int j = last; j < b_count; j++) {
            if (!used[j] && blocks[j] >= processes[i]) {
                alloc[i] = j;
                used[j] = 1;
                last = j;
                break;
            }
        }
    }
}

void best_fit() {
    for (int i = 0; i < p_count; i++) alloc[i] = -1;
    for (int j = 0; j < b_count; j++) used[j] = 0;
    for (int i = 0; i < p_count; i++) {
        int best = -1;
        for (int j = 0; j < b_count; j++)
            if (!used[j] && blocks[j] >= processes[i] && (best == -1 || blocks[j] < blocks[best]))
                best = j;
        if (best != -1) {
            alloc[i] = best;
            used[best] = 1;
        }
    }
}

void worst_fit() {
    for (int i = 0; i < p_count; i++) alloc[i] = -1;
    for (int j = 0; j < b_count; j++) used[j] = 0;
    for (int i = 0; i < p_count; i++) {
        int worst = -1;
        for (int j = 0; j < b_count; j++)
            if (!used[j] && blocks[j] >= processes[i] && (worst == -1 || blocks[j] > blocks[worst]))
                worst = j;
        if (worst != -1) {
            alloc[i] = worst;
            used[worst] = 1;
        }
    }
}

void input_data() {
    printf("Enter number of memory blocks: ");
    scanf("%d", &b_count);
    printf("Enter sizes of memory blocks: ");
    for (int i = 0; i < b_count; i++) scanf("%d", &blocks[i]);
    printf("Enter number of processes: ");
    scanf("%d", &p_count);
    printf("Enter sizes of processes: ");
    for (int i = 0; i < p_count; i++) scanf("%d", &processes[i]);
}

void print_allocation() {
    printf("\nProcess Allocation:\n");
    for (int i = 0; i < p_count; i++)
        if (alloc[i] != -1)
            printf("Process %d (%d KB) -> Block %d (%d KB)\n", i + 1, processes[i], alloc[i] + 1, blocks[alloc[i]]);
        else
            printf("Process %d (%d KB) -> Not allocated\n", i + 1, processes[i]);
}

int main() {
    input_data();
    while (1) {
        printf("\nChoose allocation algorithm:\n1. First Fit\n2. Next Fit\n3. Best Fit\n4. Worst Fit\n5. Exit\nEnter choice: ");
        int choice;
        scanf("%d", &choice);
        for (int i = 0; i < b_count; i++) used[i] = 0;
        switch (choice) {
            case 1: first_fit(); break;
            case 2: next_fit(); break;
            case 3: best_fit(); break;
            case 4: worst_fit(); break;
            case 5: return 0;
            default: printf("Invalid choice!\n"); continue;
        }
        print_allocation();
    }
    return 0;
}