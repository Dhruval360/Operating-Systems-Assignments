/*
WAP to display the directory contents. Pass the directory name from the command
line, if command line argument is not passed then the files from current working
directory needs to be displayed. [Use opendir, readdir and closedir]
Note: this programs implements "ls" command and "ls pathnameofdirname"
*/

#include<linux/limits.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>

#include<errno.h>  // Error Handling
#include<string.h> // For converting the error code held in errno to text

int main(int argc, char **argv){
    char directoryName[NAME_MAX];
    if(argc > 1) strncpy(directoryName, argv[1], NAME_MAX);
    else{
        char *name = getcwd(directoryName, NAME_MAX); // If no arguments are passed, the current directory contents must be printed
        if(name == NULL){
            perror("getcwd");
            exit(1);
        }
    } 
    /*
    getcwd has been used here so that we could print the current directory's name.
    If this isn't necessary, we could pass "." as the argument to opendir for opening the current directory
    */
    printf("Directory: %s\n\n", directoryName);
    DIR *directoryPointer = opendir(directoryName);
    if(directoryPointer == NULL){
        fprintf(stderr, "Error: %s -> %s\n", directoryName, strerror(errno));
        exit(1);
    }
    // opendir can be unsuccessful if the user does not have execute permissions for that directory (for other errors, check man page)
    // In order to open a directory, the user needs read permissions of that directory
    // In order to cd into a directory, the user needs execute permissions for that directory
    // For creating or deleting a file in a directory, the user need write permissions of that directory
    // For reading the contents of a directory, the user needs read permissions of that directory
    struct dirent *directoryEntry; // Refer to man pages of readdir to know more about this opaque struct
    printf("Inode Number\tName\n");
    while((directoryEntry = readdir(directoryPointer))) printf("%lu\t\t%s\n", directoryEntry->d_ino, directoryEntry->d_name);
    closedir(directoryPointer);
    return 0;
}