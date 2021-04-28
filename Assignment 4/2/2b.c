/*
Write program to create a link to the file “a.txt”. 
After creating a link write 10 bytes of data to one of the “a.txt” file. 
Observe the change in the size of both files and number of links. Make a note of these changes.
[Hint: use link() system call]
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>  // Error Handling
#include<string.h> // For converting the error code held in errno to text

int main(){
    char *file1 = "a.txt";
    char *file2 = "new.txt";
    int ret = link(file1, file2);
    printf("Adding a hardlink to \"%s\" from \"%s\"...", file1, file2);
    /*
    link() creates a new link (also known as a hard link) to an existing file.
    If newpath exists, it will not be overwritten.
    This new name may be used exactly as the old one for any operation; both names refer to the same file (and so have the same permissions and
    ownership) and it is impossible to tell which name was the "original".
    */
    if(ret < 0){
        printf("  Failed\n");
        fprintf(stderr, "Link creation error: %s\n", strerror(errno));
        exit(1);
    }
    printf("  Success\nOpening \"%s\"...", file2);
    int fileDescriptor = open("new.txt", O_RDWR, 0644); 
    if(fileDescriptor < 0){
        printf("  Failed\n");
        fprintf(stderr, "File opening error: %s\n", strerror(errno));
        exit(1);
    } 
    write(fileDescriptor, "Ten Bytes\n", 10); // Writing 10 bytes of data to the file
    close(fileDescriptor);
    printf("  Success\nWrote 10 bytes of data to \"%s\"\n", file2);
    return 0;
}