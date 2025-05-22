#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the block structure
typedef struct Block {
    int size;
    int is_free;
    int process_size;
    char process;
    struct Block* left;
    struct Block* right;
} Block;

// Get next power of 2 >= n
int next_power_of_two(int n) {
    int p = 1;
    while (p < n)
        p *= 2;
    return p;
}

// Create a new block
Block* create_block(int size) {
    Block* b = (Block*)malloc(sizeof(Block));
    if (!b) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    b->size = size;
    b->is_free = 1;
    b->process_size = 0;
    b->process = '-';
    b->left = NULL;
    b->right = NULL;
    return b;
}

// Allocate a process
int allocate(Block* block, int size, char name) {
    if (!block->is_free || block->size < size)
        return 0;

    int needed = next_power_of_two(size);

    if (block->size == needed && block->is_free && !block->left && !block->right) {
        block->is_free = 0;
        block->process_size = size;
        block->process = name;
        return 1;
    }

    if (!block->left && !block->right) {
        int half = block->size / 2;
        block->left = create_block(half);
        block->right = create_block(half);
    }

    if (allocate(block->left, size, name))
        return 1;

    return allocate(block->right, size, name);
}

// Try to merge buddies if both are free
void try_merge(Block* block) {
    if (!block || !block->left || !block->right)
        return;

    if (block->left->is_free && block->right->is_free &&
        !block->left->left && !block->right->right) {
        free(block->left);
        free(block->right);
        block->left = NULL;
        block->right = NULL;
        block->is_free = 1;
        block->process = '-';
        block->process_size = 0;
    }
}

// Deallocate a process by name
int deallocate(Block* block, char name) {
    if (!block)
        return 0;

    // Found the block with matching process name
    if (!block->is_free && block->process == name) {
        block->is_free = 1;
        block->process = '-';
        block->process_size = 0;
        return 1;
    }

    int found = 0;
    if (block->left)
        found = deallocate(block->left, name);

    if (!found && block->right)
        found = deallocate(block->right, name);

    if (found)
        try_merge(block);

    return found;
}

// Print blocks with indentation
void print_blocks(Block* block, int level) {
    if (!block)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("%d", block->size);

    if (!block->is_free && block->process != '-') {
        int frag = block->size - block->process_size;
        printf(" (%c, Frag: %d)", block->process, frag);
    }

    printf("\n");

    print_blocks(block->left, level + 1);
    print_blocks(block->right, level + 1);
}

// Free the memory tree
void free_blocks(Block* block) {
    if (!block)
        return;
    free_blocks(block->left);
    free_blocks(block->right);
    free(block);
}

// Main function
int main() {
    int choice, process_count;
    Block* root = create_block(1024);

    while (1) {
        printf("\n--- Buddy System Menu ---\n");
        printf("1. Allocate Process\n");
        printf("2. Deallocate Process\n");
        printf("3. Print Memory Blocks\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char name;
            int size;
            printf("Process name: ");
            scanf(" %c", &name);
            printf("Process size: ");
            scanf("%d", &size);

            if (!allocate(root, size, name)) {
                printf("Could not allocate memory for process %c\n", name);
            } else {
                printf("Process %c allocated successfully.\n", name);
            }
        } else if (choice == 2) {
            char name;
            printf("Enter process name to deallocate: ");
            scanf(" %c", &name);

            if (!deallocate(root, name)) {
                printf("Process %c not found in memory.\n", name);
            } else {
                printf("Process %c deallocated successfully.\n", name);
            }
        } else if (choice == 3) {
            printf("\nMemory Allocation:\n");
            print_blocks(root, 0);
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    free_blocks(root);
    return 0;
}
