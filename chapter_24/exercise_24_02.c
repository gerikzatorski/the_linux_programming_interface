/* exercise_20_02.char */

/*********************************************************************
Write a program to demonstrate that after a vfork(), the child process
can close a file descriptor (e.g., descriptor 0) without affecting the
corresponding file descriptor in the parent.
*********************************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int fd;
    char template[] = "/tmp/testXXXXXX";

    fd = mkstemp(template);
    if (fd == -1) {
        perror("mkstemp");
        exit(EXIT_FAILURE);
    }

    switch(fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        if (close(fd) == -1) {
            perror("exit");
            _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    default:
        /* This should fail if child affects corresponding fd */
        if (close(fd) == -1) {
            perror("exit");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}
