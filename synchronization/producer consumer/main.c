#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define n 10

typedef sem_t semaphore;
semaphore s;
semaphore empty;
semaphore full;
int in = 0, out = 0;
int buf[n] = {0};

int produce_item() {
    return rand() % 100;
}

void insert_item(int item) {
    buf[in] = item;
    in = (in + 1) % n;
}

int remove_item() {
    int item = buf[out];
    out = (out + 1) % n;
    return item;
}

void* producer(void* arg) {
    while(1) {
        int item = produce_item();
        sem_wait(&empty);
        sem_wait(&s);
        insert_item(item);
        printf("Produced item: %d\n", item);
        sem_post(&s);
        sem_post(&full);
        //sleep(1);
    }
}

void* consumer(void* arg) {
    while(1) {
        sem_wait(&full);
        sem_wait(&s);
        int item = remove_item();
        printf("Consumed item: %d\n", item);
        sem_post(&s);
        sem_post(&empty);
        //sleep(1);
    }
}

int main() {
    pthread_t pro, con;
    sem_init(&s, 0, 1);
    sem_init(&empty, 0, n);
    sem_init(&full, 0, 0);
    pthread_create(&pro, NULL, producer, NULL);
    pthread_create(&con, NULL, consumer, NULL);
    pthread_join(pro, NULL);
    pthread_join(con, NULL);
    sem_destroy(&s);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
