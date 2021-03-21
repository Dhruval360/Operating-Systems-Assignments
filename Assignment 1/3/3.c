/*
Create a simple pipe between parent and child using pipe() system call. The child process
writes information to the write end of the pipe and the parent process reads information from
the read end of the pipe and display it to standard output device.
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv){
    int fd[2];
    int success = pipe(fd); // Creating a pipe
    if(success < 0){ // pipe returns -1 if not successful 
        fprintf(stderr, "Failed to create pipe\n");
        exit(-1);
    }
    
    int process_id = fork(); // Trying to create a child process
    if(process_id < 0){ // Failed to create the child process
        fprintf(stderr, "Fork failed\n"); 
        exit(-1);
    }
    else if(process_id == 0){ // Child process (fork returns 0 to the child process)
        printf("This is the child process\n");
        printf("String written to pipe:\n%s\nString length = %lu\n", argv[1], strlen(argv[1]));
        write(fd[1], argv[1], strlen(argv[1])); // Writing to the write end of the pipe
        close(fd[1]);
    }
    else{ // Parent process (fork returns the process id of the child process to the parent process)
        usleep(200); // Wait for some time for the child process to write to the pipe
        char buffer[100]; // Will be used to read from characters from the pipe
        printf("\n------------------------------------\n");
        printf("This is the parent process\nBuffer length before reading from pipe = %lu\nString read from pipe:\n", strlen(buffer));
        read(fd[0], buffer, sizeof(buffer)); // Reading from the read end of the pipe into the charcter array named buffer
        write(1, buffer, strlen(buffer)); // Writing from the buffer onto stdout
        printf("\nBuffer length after reading from pipe = %lu\n", strlen(buffer));
        close(fd[0]);
    }
    return 0;
}