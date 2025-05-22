#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_READERS 5
#define MAX_WRITERS 2

sem_t mutex;
sem_t readCountAccess;
int readCount = 0;

void* reader(void* arg) 
{
    int id = *(int*)arg;
    
    while (1) 
    {
        sem_wait(&readCountAccess);
        readCount++;
        if (readCount == 1) 
            sem_wait(&mutex);
        sem_post(&readCountAccess);
        
        // Reading section
        printf("Reader %d is reading\n", id);
        sleep(1);
        
        sem_wait(&readCountAccess);
        readCount--;
        if (readCount == 0) 
            sem_post(&mutex);
        sem_post(&readCountAccess);

        sleep(rand() % 3);
    }
    
    return NULL;
}

void* writer(void* arg) 
{
    int id = *(int*)arg;
    
    while (1) 
    {
        sem_wait(&mutex);

        // Writing section
        printf("Writer %d is writing\n", id);
        sleep(2);

        sem_post(&mutex);

        sleep(rand() % 5);
    }

    return NULL;
}

int main(void) 
{
    pthread_t readers[MAX_READERS], writers[MAX_WRITERS];
    int reader_ids[MAX_READERS], writer_ids[MAX_WRITERS];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&readCountAccess, 0, 1);

    // Create reader threads
    for (int i = 0; i < MAX_READERS; i++) 
    {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < MAX_WRITERS; i++) 
    {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Wait for all threads (though they run indefinitely)
    for (int i = 0; i < MAX_READERS; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < MAX_WRITERS; i++)
        pthread_join(writers[i], NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&readCountAccess);

    return 0;
}
