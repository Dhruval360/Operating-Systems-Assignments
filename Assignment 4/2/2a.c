/*
Write a program to set the size of the existing file to 0 bytes without creating new file.
[Hint: use open() and lseek() system calls]
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h> 

int main(){
    char buffer[5];
    char *fileName = "file.txt";
    printf("Opening \"%s\"...", fileName);
    int fileDescriptor = open(fileName, O_TRUNC, 0644); // Truncates the opened file (reduces its size to 0)
    /*
    If O_RDWR or O_WRONLY is used instead of O_TRUNC and data lesser than the original file size is written,
    the file will still retain its original size with some data overwritten.
    Any content that wasn't overwritten will still be retained in the file.
    */
    if(fileDescriptor < 0){
        printf("  Failed\n");
        perror("File opening error");
        exit(1);
    }
    printf("  Success\nTruncated \"%s\" successfully\n", fileName); 
    close(fileDescriptor);
    return 0;
}