/* exercise_05_02.c */

/*********************************************************************
Write a program that opens an existing file for writing with the
O_APPEND flag, and then seeks to the beginning of the file before
writing some data. Where does the data appear in the file? Why?
*********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int flags, fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    flags = O_APPEND | O_WRONLY;
    fd = open(argv[1], flags);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    
    if(write(fd, "abc\n", strlen("abc\n")) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
