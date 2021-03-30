/*
Write a multithreaded C program that calculates various statistical values for a list of
numbers. This program will be passed a series of numbers on the command line and will then
create three separate worker threads. One thread will determine the average of the numbers,
the second will determine the maximum value, and the third will determine the minimum value.
For example, suppose your program is passed the integers 90 81 78 95 79 72 85. The program
will report:
The average value is 82
The minimum value is 72
The maximum value is 95
The variables representing the average, minimum and maximum values will be stored
globally. The worker threads will set these values, and the parent thread will output the
values once the workers have exited.
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

// Global variables that the individual threads will update
float avg;
int min, max;

typedef struct thread_args{ // A structure for passing arguments to the threads
    int *array;
    int n;
}thread_args;

void average(void *input){ // Calculates the average of an array of integers
    float sum = 0;
    for(int i = 0; i < ((thread_args*)input)->n; i++) sum += ((thread_args*)input)->array[i];
    avg = sum/((thread_args*)input)->n;
}

void maximum(void *input){ // Finds the maximum element of an array 
    max = ((thread_args*)input)->array[0];
    for(int i = 1; i < ((thread_args*)input)->n; i++) if(((thread_args*)input)->array[i] > max) max = ((thread_args*)input)->array[i];
}

void minimum(void *input){ // Finds the minimum element of an array
    min = ((thread_args*)input)->array[0];
    for(int i = 1; i < ((thread_args*)input)->n; i++) if(((thread_args*)input)->array[i] < min) min = ((thread_args*)input)->array[i];
}

int main(int argc, char **argv){
    pthread_t threads[3]; // Creating an array of three thread identifiers
    
    thread_args *args = (thread_args*)malloc(sizeof(thread_args)); // This will be passed as the arguments to each individual thread
    args->array = (int*)malloc(sizeof(int) * (argc - 1));
    args->n = argc - 1;
    for(int i = 1; i<argc; i++) args->array[i-1] = atoi(argv[i]); // Converting the command line strings into integers
    
    int ret;
    ret = pthread_create(&threads[0], NULL, (void*)average, (void*)args);
    if(ret){
        fprintf(stderr, "Error code from pthread_create() is %d\n", ret);
        free(args->array);
        free(args);
        exit(-1);
    } 

    ret = pthread_create(&threads[1], NULL, (void*)maximum, (void*)args);
    if(ret){
        fprintf(stderr, "Error code from pthread_create() is %d\n", ret);
        free(args->array);
        free(args);
        exit(-1);
    } 

    ret = pthread_create(&threads[2], NULL, (void*)minimum, (void*)args);
    if(ret){
        fprintf(stderr, "Error code from pthread_create() is %d\n", ret);
        free(args->array);
        free(args);
        exit(-1);
    } 

    for(int i = 0; i < 3; i++) pthread_join(threads[i], NULL); // Waiting for all the threads to finish their work

    printf("The minimum of the given list of integers is: %d\n", min);
    printf("The maximum of the given list of integers is: %d\n", max);
    printf("The average of the given list of integers is: %f\n", avg);

    free(args->array);
    free(args);
    pthread_exit(NULL);
}