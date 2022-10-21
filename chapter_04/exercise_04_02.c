/* exercise_04_02.c */

/*********************************************************************
Write a program like cp that, when used to copy a regular file that
contains holes (sequences of null bytes), also creates corresponding
holes in the target file.
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/* Allow "cc -D" to override definition */

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int input_fd, ouput_fd, output_flags;
    mode_t output_perms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s old-file new-file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_fd = open(argv[1], O_RDONLY);
    if (input_fd == -1) {
        perror("open (input file)");
        exit(EXIT_FAILURE);
    }
    
    output_flags = O_CREAT | O_WRONLY | O_TRUNC;
    output_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                   S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    ouput_fd = open(argv[2], output_flags, output_perms);
    if (ouput_fd == -1) {
        perror("open (output file)");
        exit(EXIT_FAILURE);
    }

    while ((numRead = read(input_fd, buf, BUF_SIZE)) > 0) {
        if (write(ouput_fd, buf, numRead) != numRead) {
            perror("write");
            break;
        }
    }

    if (numRead == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (close(input_fd) == -1) {
        perror("close (input file)");
        exit(EXIT_FAILURE);
    }

    if (close(ouput_fd) == -1) {
        perror("close (output file)");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
