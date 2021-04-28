/*
WAP to display the directory contents. Pass the directory name from the command
line, if command line argument is not passed then the files from current working
directory needs to be displayed. [Use opendir, readdir and closedir]
Note: this programs implements ls command and ls pathnameofdirname
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
    if(argc > 1) strcpy(directoryName, argv[1]);
    else{
        char *name = getcwd(directoryName, NAME_MAX); // If no arguments are passed, the current directory contents must be printed
        if(name == NULL){
            fprintf(stderr, "getcwd error message: %s\n", strerror(errno));
            exit(1);
        }
    }
    printf("Directory: %s\n\n", directoryName);
    DIR *directoryPointer = opendir(directoryName);
    if(directoryPointer == NULL){
        fprintf(stderr, "Error: %s -> %s\n", directoryName, strerror(errno));
        exit(1);
    }
    struct dirent *directoryEntry; // Refer to man pages of readdir to know more about this opaque struct
    printf("Inode Number\tName\n");
    while((directoryEntry = readdir(directoryPointer))){
        printf("%lu\t\t%s\n", directoryEntry->d_ino, directoryEntry->d_name);
    }
    closedir(directoryPointer);
    return 0;
}