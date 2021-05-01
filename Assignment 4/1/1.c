/*
Write an application program to write 20 bytes of data to a file and display the first 5 bytes and last 5 bytes of data from the file. 
Also, print the size of the file using lseek() system call. [Hint: use open(), read(), write(), lseek(), close() system calls]
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
    char buffer[5];
    char *fileName = "file.txt";
    int returnValue; // Used for catching the return values of all the fucntion calls used in this program
    printf("Opening \"%s\"...", fileName);
    int fileDescriptor = open(fileName, O_RDWR|O_CREAT, 0644); 
    /*
    File permissions are in octal. Hence, 0644 is expanded as: 000 110 100 100                       
                                                  (Sticky bit) (Owner) (Group Members) (Others) 
    The three bits are Read Write Execute (rwx) {Use ls -l to see them}
    Sticky bit only applies to directories.
    When a directory's sticky bit is set, the filesystem treats the files in such directories in a special way
    so only the file's owner, the directory's owner, or root user can rename or delete the file. 
    Without the sticky bit set, any user with write and execute permissions for the directory can rename or delete contained files. 
    Typically this is set on the /tmp directory to prevent ordinary users from deleting or moving other users' files.

    O_RDONLY => Opens the file in read only mode
    O_WRONLY => Opens the file in write only mode
    O_RDWR => Opens the file in read write mode
    O_CREAT => Creates the file if it doesn't exist
    O_APPEND => Appends to the existing file
    O_TRUNC => Truncates the file (overwrites)
    O_EXCL => Tells the open function to fail if the file already exists (a method to prevent overwriting)
    Refer to man pages for more info

    If a file has been opened in readonly mode and we try to write to it, a "Bad file descriptor" error is raised
    If permissions are not allowing for a user to open or write to the file, a "Permission denied" error is raised
    */
    if(fileDescriptor == -1){
        printf("  Failed\n");
        perror("File opening error"); // Similar to fprintf(stderr, "File opening error: %s\n", strerror(errno));
        /*
        If we use fprintf, we need to include errno.h and string.h 
        The  <errno.h>  header  file defines the integer variable errno.
        This variable is set by system calls and some library functions in the event of an error to indicate what went wrong.
        <string.h> header file contains a method called strerror(error_code) that translates error_code into text.
        
        perror does this internally and hence, we need not do it ourselves
        */
        exit(1);
    }
    printf("  Success\n");
    /*
        Note: The fileDescriptor value returned will be the smallest available value
        Under normal circumstances every UNIX program has three streams opened for it when it starts up, 
            * Input                                 (0 -> stdin)
            * Output                                (1 -> stdout)
            * Printing diagnostic or error messages (2 -> stderr) 
        These are typically attached to the user's terminal (see tty(4)) but might instead refer  to  files or other devices, 
        depending on what the parent process chose to set up.  (See also the "Redirection" section of sh(1).)

        Hence, the first file opened after this will have the value 3 (i.e, fileDescriptor will have the value 3 if it opened successfully).
        In case we close any of the 3 default streams, the newly opened file will have that value.
        Files can be closed using the close() method.
    */

    returnValue = lseek(fileDescriptor, 0, SEEK_END); // Seeking to the beginning of the file
    if(returnValue == -1){ // lseek() returns -1 if it failed
        perror("lseek error");
        exit(1);
    } 
    printf("The size of the file before writing any data is \"%s\" is %d bytes\n", fileName, returnValue);
    /*
    Upon successful completion, lseek() returns the resulting offset location as measured in bytes from the beginning of the file
    
    SEEK_SET = Apply the offset to the beginning of the file    (Return value = 0 if offset is 0)
    SEEK_CUR = Apply the offset to the current cursor position  
    SEEK_END = Apply the offset to the end of the file          (Return value = size of file in bytes if offset is 0)
    
    If there is an error in the offset values given to lseek (which might give it instructions to seek outside the file),
    an "Invalid argument" error is raised
    */

    returnValue = lseek(fileDescriptor, 0, SEEK_SET); // Seeking to the beginning of the file
    if(returnValue == -1){ 
        perror("lseek error");
        exit(1);
    } 

    returnValue = write(fileDescriptor, "Twenty bytes of data", 20); // Writing 20 bytes of data to the file
    if(returnValue == -1){
        perror("File writing error"); // write() returns -1 if it failed and number of bytes written (<= what is specified) if its successfull
        exit(1);
    }

    returnValue = lseek(fileDescriptor, 0, SEEK_SET); // Seeking to the beginning of the file
    if(returnValue == -1){ 
        perror("lseek error");
        exit(1);
    } 

    returnValue = read(fileDescriptor, buffer, 5);    // Reading the first 5 bytes of file into the buffer
    if(returnValue == -1){
        perror("File reading error"); // read() returns -1 if it failed and number of bytes read (<= what is specified) if its successfull
        exit(1);
    }
    // Note: If the buffer is not big enough, stack smashing occurs (indicating a buffer overflow)
    
    // Writing to stdout (1)
    returnValue = write(1, "The first 5 bytes of the file are: \"", 36);
    if(returnValue == -1){
        perror("Error writing to stdout");
        exit(1);
    }
    
    returnValue = write(1, buffer, 5); 
    if(returnValue == -1){
        perror("Error writing to buffer");
        exit(1);
    }
    
    returnValue = write(1, "\"\n", 2);
    if(returnValue == -1){
        perror("Error writing to stdout");
        exit(1);
    }

    returnValue = lseek(fileDescriptor, -5, SEEK_END); // Seeking to the 5th byte from the end of the file
    if(returnValue == -1){
        perror("lseek error");
        exit(1);
    }

    returnValue = read(fileDescriptor, buffer, 5);     // Reading the last 5 bytes of file into the buffer
    
    // Writing to stdout (1)
    returnValue = write(1, "The last 5 bytes of the file are:  \"", 36);
    if(returnValue == -1){
        perror("Error writing to stdout");
        exit(1);
    }

    returnValue = write(1, buffer, 5);    
    if(returnValue == -1){
        perror("Error writing to stdout");
        exit(1);
    }

    returnValue = write(1, "\"\n", 2);
    if(returnValue == -1){
        perror("Error writing to stdout");
        exit(1);
    }

    returnValue = lseek(fileDescriptor, 0, SEEK_END); // Seeking to the beginning of the file
    if(returnValue == -1){ 
        perror("lseek error");
        exit(1);
    } 
    printf("The size of the file after writing data to it is \"%s\" is %d bytes\n", fileName, returnValue);

    close(fileDescriptor); // Closing the file
    return 0;
}