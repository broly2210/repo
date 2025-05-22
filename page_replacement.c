#include <stdio.h>
#include <stdlib.h>
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
while (p < n) p *= 2;
return p;
}
// Create a new block
Block* create_block(int size) {
Block* b = (Block*)malloc(sizeof(Block));
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
if (block->size == needed && block->is_free) {
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
if (allocate(block->left, size, name)) return 1;
return allocate(block->right, size, name);
}
// Print blocks with indentation
void print_blocks(Block* block, int level) {
if (!block) return;
for (int i = 0; i < level; i++) printf(" ");
printf("%d", block->size);
if (!block->is_free && block->process != '-') {
int frag = block->size - block->process_size;
printf(" (%c, Frag: %d)", block->process, frag);
}
printf("\n");
print_blocks(block->left, level + 1);
print_blocks(block->right, level + 1);
}
// Main function
int main() {
int n;
Block* root = create_block(1024);
printf("Enter number of processes: ");
scanf("%d", &n);
for (int i = 0; i < n; i++) {
char name;
int size;
printf("Process %d name: ", i + 1);
scanf(" %c", &name);
printf("Process %d size: ", i + 1);
scanf("%d", &size);
if (!allocate(root, size, name)) {
printf("Could not allocate memory for process %c\n", name);
}
}
printf("\nMemory Allocation:\n");
print_blocks(root, 0);
return 0;
}