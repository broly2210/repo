#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>  
#include <unistd.h>   

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t mutex;
sem_t empty;
sem_t full;

void produce() 
{
    int item = rand() % 100;
    printf("Produced: %d\n", item);
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
}

void consume() 
{
    int item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    printf("Consumed: %d\n", item);
}

void* producer(void* arg) 
{
    int counter = 0;
    while (counter < BUFFER_SIZE) 
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        produce();
        sem_post(&mutex);
        sem_post(&full);
        counter++;
        sleep(1); 
    }
    return NULL;
}

void* consumer(void* arg) 
{
    int counter = 0;
    while (counter < BUFFER_SIZE) 
    {
        sem_wait(&full);
        sem_wait(&mutex);
        consume();
        sem_post(&mutex);
        sem_post(&empty);
        counter++;
        sleep(1); 
    }
    return NULL;
}

int main(void)
{
    sem_init(&mutex, 0, 1);  
    sem_init(&empty, 0, BUFFER_SIZE);  
    sem_init(&full, 0, 0);   

    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}