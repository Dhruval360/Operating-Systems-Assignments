/*
Write a program to create a child process which calls one of the exec functions to create a
file named "abc.txt". Parent using one of the exec calls must execute the command "ls". 
Make the parent wait for the child to terminate and then execute "ls". 
Do not use "system" function
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    int process_id = fork(); // Trying to create a child process
    if(process_id < 0){ // Failed to create the child process
        fprintf(stderr, "Fork failed\n"); 
        exit(-1);
    }
    else if(process_id == 0){ // Child process (fork returns 0 to the child process)
        printf("This is the child process\ntouch abc.txt\n");
        execl("/bin/touch", "touch", "abc.txt", NULL);
        //    Command path,  arg[0],    arg[1], NULL  
        //               (command name)
    }
    else{ // Parent process (fork returns the process id of the child process to the parent process)
        wait(NULL); // Waiting for the child process to terminate
        printf("\n------------------------------------\n");
        printf("This is the parent process\nls -l\n");
        execl("/bin/ls", "ls", "-l", NULL); // ls command with -l argument to use a long listing format
    }
    return 0;
}