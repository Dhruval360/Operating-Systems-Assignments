/*
Write a program to set the size of the existing file to 0 bytes without creating new file.
[Hint: use open() and lseek() system calls]
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h> 
#include<errno.h>  // Error Handling
#include<string.h> // For converting the error code held in errno to text

int main(){
    char buffer[5];
    char *fileName = "file.txt";
    printf("Opening \"%s\"...", fileName);
    int fileDescriptor = open(fileName, O_TRUNC, 0644); // Truncates the opened file (reduces its size to 0)
    if(fileDescriptor < 0){
        printf("  Failed\n");
        fprintf(stderr, "File opening error: %s\n", strerror(errno));
        exit(1);
    }
    printf("  Success\nTruncated \"%s\" successfully\n", fileName); 
    close(fileDescriptor);
    return 0;
}