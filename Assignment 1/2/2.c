/*
Write a program where in the child process initiates a new program which finds the sum of
n numbers. The numbers to add are given as arguments in the exec function. Use appropriate
exec function. Parent process should wait for the termination of child process.
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char **argv){
    int process_id = fork(); // Trying to create a child process
    if(process_id < 0){ // Failed to create the child process
        fprintf(stderr, "Fork failed\n"); 
        exit(-1);
    }
    else if(process_id == 0){ // Child process (fork returns 0 to the child process)
        printf("This is the child process\n");
        int process_id = fork(); // Trying to create another child process
        if(process_id < 0){ // Failed to create the child process
            fprintf(stderr, "Fork failed\n"); 
            exit(-1);
        }
        else if(process_id == 0){ // Child process (fork returns 0 to the child process)
            printf("------------------------------------------------------------------------------------------\n");
            printf("This is the child's child process. It compiles the Add program using: gcc -o Add.out Add.c\n");
            execl("/bin/gcc", "gcc", "-o", "Add.out", "./Add.c", NULL);
        }
        else{ // Parent process (fork returns the process id of the child process to the parent process)
            wait(NULL); // Waiting for the child process to terminate
            printf("------------------------------------------------------------------------------------------\n");
            printf("This is the child process that will execute the program using: ./Add.out ");
            for(int i = 1; i<argc; i++) printf("%s ", argv[i]); // Printing the arguments of the main program
            printf("\n");
            execv("./Add.out", argv); // Passing the command line arguments list of the parent process as command line arguments to the child process
        }
    }
    else{ // Parent process (fork returns the process id of the child process to the parent process)
        wait(NULL); // Waiting for the child process to terminate
        printf("------------------------------------------------------------------------------------------\n");
        printf("This is the parent process\n");
    }
    return 0;
}