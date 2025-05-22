#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

// Each chopstick is represented as a binary semaphore.
sem_t chopsticks[NUM_PHILOSOPHERS];

void* philosopher(void* arg)
{
    int id = *(int*) arg;
    int left = id; // left chopstick number
    int right = (id + 1) % NUM_PHILOSOPHERS; // right chopstick number

    while (1) 
    {
        printf("Philosopher %d is thinking.\n", id);

        // To avoid deadlock, we use different ordering for even and odd IDs.
        if (id % 2 == 0) 
        {
            // Even-numbered philosopher: pick up right chopstick first.
            printf("Philosopher %d is trying to pick up chopstick %d (right).\n", id, right);
            sem_wait(&chopsticks[right]);
            printf("Philosopher %d picked up chopstick %d (right).\n", id, right);

            printf("Philosopher %d is trying to pick up chopstick %d (left).\n", id, left);
            sem_wait(&chopsticks[left]);
            printf("Philosopher %d picked up chopstick %d (left).\n", id, left);
        } 
        else 
        {
            // Odd-numbered philosopher: pick up left chopstick first.
            printf("Philosopher %d is trying to pick up chopstick %d (left).\n", id, left);
            sem_wait(&chopsticks[left]);
            printf("Philosopher %d picked up chopstick %d (left).\n", id, left);

            printf("Philosopher %d is trying to pick up chopstick %d (right).\n", id, right);
            sem_wait(&chopsticks[right]);
            printf("Philosopher %d picked up chopstick %d (right).\n", id, right);
        }

        // Now that both chopsticks are in hand, the philosopher eats.
        printf("Philosopher %d is eating.\n", id);
        sleep(2);

        // After eating, the philosopher puts down both chopsticks.
        sem_post(&chopsticks[left]);
        sem_post(&chopsticks[right]);
        printf("Philosopher %d has put down chopsticks %d and %d.\n\n", id, left, right);
    }
    
    return NULL;
}

int main() 
{
    pthread_t threads[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize the semaphores for each chopstick (each with a value of 1).
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) 
    {
        sem_init(&chopsticks[i], 0, 1);
    }

    // Create one thread per philosopher.
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) 
    {
        philosopher_ids[i] = i;
        if (pthread_create(&threads[i], NULL, philosopher, &philosopher_ids[i]) != 0) 
        {
            perror("Failed to create philosopher thread");
            exit(EXIT_FAILURE);
        }
    }

    // Join the threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) 
    {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
