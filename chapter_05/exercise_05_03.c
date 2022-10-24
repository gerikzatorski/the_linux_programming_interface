/* exercise_05_03.c */

/*********************************************************************
This exercise is designed to demonstrate why the atomicity guaranteed
by opening a file with the O_APPEND flag is necessary. Write a program
that takes up to three command-line arguments:

$ atomic_append filename num-bytes [x]

This file should open the specified filename (creating it if
necessary) and append num-bytes bytes to the file by using write() to
write a byte at a time. By default, the program should open the file
with the O_APPEND flag, but if a third command-line argument (x) is
supplied, then the O_APPEND flag should be omitted, and instead the
program should perform an lseek(fd, 0, SEEK_END) call before each
write(). Run two instances of this program at the same time without
the x argument to write 1 million bytes to the same file:

$ atomic_append f1 1000000 & atomic_append f1 1000000

Repeat the same steps, writing to a different file, but this time
specifying the x argument:

$ atomic_append f2 1000000 x & atomic_append f2 1000000 x

List the sizes of the files f1 and f2 using ls –l and explain the
difference.
*********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[])
{
    bool atomic = true;
    int flags, fd, num_bytes, i;
    char c[1];

    if (argc == 4) {
        if (strcmp(argv[3], "x") == 0) {
            atomic = false;
        }
        else {
            fprintf(stderr, "Usage: %s filename num-bytes [x]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (argc != 3) {
        fprintf(stderr, "Usage: %s filename num-bytes [x]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    num_bytes = atoi(argv[2]); // TODO: validate input

    if (atomic) {
        flags = O_APPEND | O_WRONLY;
    }
    else {
        flags = O_WRONLY;
    }
    
    fd = open(argv[1], flags);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < num_bytes; i++) {
        /* repeating "abc"  */
        c[0] = (i % 3) + 97;

        if (!atomic) {
            if (lseek(fd, 0, SEEK_END) == -1) {
                perror("lseek");
                exit(EXIT_FAILURE);
            }
        }

        if(write(fd, c, 1) != 1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
