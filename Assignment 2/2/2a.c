/*
Write a C program to simulate race condition in Producer Consumer Problem.
Note: Implement a main program that creates two threads: producer and consumer threads
which execute producer and consumer functions, respectively. The producer should produce
an item and update the buffer. The consumer should consume an item and update the buffer.
You can use bounded buffer and both the producer and consumer threads can be infinite loops.
Show how race condition occurs between producer and consumer without mutual exclusion.
*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define BUFFER_SIZE 100
#define PAIRS 2 // Pairs of producers and consumers. More producer consumer pairs makes it easier to spot the race condition

int *buffer;
int item = 0, front = 0, rear = 0, counter = 0; // counter is the number of items in the buffer

void produce(void *pid){
    while(1){
        while(counter == BUFFER_SIZE); // Buffer full, so stop producing
        usleep(100000); // Simulating some time consuming calculations
        counter++;
        buffer[rear] = item;
        rear = (rear + 1) % BUFFER_SIZE;
        printf("Producer %d: Produced item %d + | Items in buffer = %d\n", *(int*)pid, ++item, counter);
    }
}

void consume(void *pid){
    while(1){
        while(counter == 0); // Buffer empty, so stop consuming
        usleep(100000); // Simulating some time consuming calculations
        counter--;
        printf("Consumer %d: Consumed item %d - | Items in buffer = %d\n", *(int*)pid, buffer[front], counter);
        front = (front + 1) % BUFFER_SIZE;
    }
}

int main(){
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
    pthread_exit(NULL);
}