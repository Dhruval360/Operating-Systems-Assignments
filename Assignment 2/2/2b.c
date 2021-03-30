/*
Write a C program to implement Producer Consumer problem using Semaphores.
Note: Implement a main program that creates two threads: producer and consumer threads
which execute producer and consumer functions, respectively. The producer should produce
an item and update the buffer. The consumer should consume an item and update the buffer.
Use Semaphores to enclose the critical sections in both producer and consumer so that only
one of them can update the buffer at a time and prevent race condition. 
Consumer should wait if buffer is empty and producer should signal when the
buffer has at least one item. You can use bounded buffer.
*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h> 
#include<stdlib.h>
#include<unistd.h>
#define BUFFER_SIZE 100
#define PAIRS 1 // To change the pairs of producers and consumers

sem_t semaphore; 
int *buffer;
int item = 0, front = 0, rear = 0, counter = 0; // counter is the number of items in the buffer

void produce(void *pid){
    while(1){
        while(counter == BUFFER_SIZE); // Buffer full, so stop producing
        sem_wait(&semaphore); // Decrement semaphore if possible, else block
        usleep(10000); // Simulating some time consuming calculations
        printf("Producer %d: Produced item %d + | Items in buffer = %d\n", *(int*)pid, ++item, ++counter);
        buffer[rear] = item;
        rear = (rear + 1) % BUFFER_SIZE;
        sem_post(&semaphore); // Increment semaphore
    }
}

void consume(void *pid){
    while(1){
        while(counter == 0); // Buffer empty, so stop consuming
        sem_wait(&semaphore); // Decrement semaphore if possible, else block        
        usleep(10000); // Simulating some time consuming calculations
        printf("Consumer %d: Consumed item %d - | Items in buffer = %d\n", *(int*)pid, buffer[front], --counter);
        front = (front + 1) % BUFFER_SIZE;
        sem_post(&semaphore); // Increment semaphore
    }
}

int main(){
    sem_init(&semaphore, 0, 1); 
    buffer = (int*)malloc(sizeof(int) * BUFFER_SIZE);
    pthread_t threads[PAIRS * 2];
    int *pids = (int*)malloc(sizeof(int) * BUFFER_SIZE);
    
    int ret, j = 1;    
    for(int i = 0; i < PAIRS * 2; i += 2){
        pids[i] = pids[i+1] = j++; 
        ret = pthread_create(&threads[i], NULL, (void*)produce, &pids[i]); // Producer thread
        if(ret){
            fprintf(stderr, "The error value returned by pthread_create() is %d\n", ret);
            exit(-1);
        }

        ret = pthread_create(&threads[i+1], NULL, (void*)consume, &pids[i+1]); // Consumer thread
        if(ret){
            fprintf(stderr, "The error value returned by pthread_create() is %d\n", ret);
            exit(-1);
        }
    }
    
    for(int i = 0; i<PAIRS; i++) pthread_join(threads[i], NULL);
    free(buffer);
    free(pids);
    sem_destroy(&semaphore); 
    pthread_exit(NULL);
}