/*
Write an application program to write 20 bytes of data to a file and display the first 5 bytes and last 5 bytes of data from the file. 
Also, print the size of the file using lseek() system call. [Hint: use open(), read(), write(), lseek(), close() system calls]
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
    int fileDescriptor = open(fileName, O_RDWR|O_CREAT, 0644); 
    /*
    File permissions are in octal. Hence, 0644 is expanded as: 000 110 100 100                       
                                                  (Sticky bit) (Owner) (Group Members) (Others) 
    The three bits are Read Write Execute (rwx) {Use ls -l to see them}
    Sticky bit is not used anymore?

    O_RDWR => Opens the file in read write mode
    O_CREAT => Creates the file if it doesn't exist
    O_APPEND => Appends to the existing file
    O_TRUNC => Truncates the file (overwrites)
    Refer to man pages for more info
    */
    if(fileDescriptor < 0){
        printf("  Failed\n");
        fprintf(stderr, "File opening error: %s\n", strerror(errno));
        exit(1);
    }
    printf("  Success\n");
    /*
        Note: The fileDescriptor value returned will be the smallest available value
        Under normal circumstances every UNIX program has three streams opened for it when it starts up, 
            * one for input                                 (0 -> stdin)
            * one for output                                (1 -> stdout)
            * one for printing diagnostic or error messages (2 -> stderr) 
        These are typically attached to the user's terminal (see tty(4)) but might instead refer  to  files or other devices, 
        depending on what the parent process chose to set up.  (See also the "Redirection" section of sh(1).)

        Hence, the first file opened after this will have the value 3 (i.e, fileDescriptor will have the value 3 if it opened successfully).
        In case we close any of the 3 default files, the newly opened file will have that value.
        Files can be closed using the close() method.
    */
    write(fileDescriptor, "Twenty bytes of data", 20); // Writing 20 bytes of data to the file
    
    lseek(fileDescriptor, 0, SEEK_SET); // Seeking to the beginning of the file
    read(fileDescriptor, buffer, 5);    // Reading the first 5 bytes of file into the buffer
    
    // Writing to stdout (1)
    write(1, "The first 5 bytes of the file are: \"", 36);
    write(1, buffer, 5); 
    write(1, "\"\n", 2);

    lseek(fileDescriptor, -5, SEEK_END); // Seeking to the 5th byte from the end of the file
    read(fileDescriptor, buffer, 5);     // Reading the last 5 bytes of file into the buffer
    
    // Writing to stdout (1)
    write(1, "The last 5 bytes of the file are:  \"", 36);
    write(1, buffer, 5);    
    write(1, "\"\n", 2);

    close(fileDescriptor); // Closing the file
    return 0;
}