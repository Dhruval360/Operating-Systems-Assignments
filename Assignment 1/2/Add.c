// A helper program that finds the sum of n numbers given as a command line argument
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
    int sum = 0;
    for(int i = 1; i < argc; i++){
        int number = atoi(argv[i]);
        sum += number;
        printf("%d + ", number);
    } 
    printf("\b\b= %d\n", sum);
    return 0;
}