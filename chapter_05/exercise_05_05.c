/* exercise_05_05.c */

/*********************************************************************
Write a program to verify that duplicated file descriptors share a
file offset value and open file status flags.
*********************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd1, fd2, flags0, flags1, flags2;
    off_t offset1, offset2;

    flags0 = O_CREAT | O_WRONLY | O_TRUNC;
    fd1 = open("filedups", flags2);
    fd2 = dup(fd1);

    offset1 = lseek(fd1, 0, SEEK_CUR);
    if (offset1 == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
    offset2 = lseek(fd2, 0, SEEK_CUR);
    if (offset2 == -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    assert(offset1 == offset2);

    flags1 = fcntl(fd1, F_GETFL);
    if (flags1 == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    flags2 = fcntl(fd2, F_GETFL);
    if (flags2 == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    
    assert(flags1 == flags2);
    
    exit(EXIT_SUCCESS);
}
