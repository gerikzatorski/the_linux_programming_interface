/* exercise_26_01.c */

/*********************************************************************
Write a program to verify that when a child’s parent terminates, a
call to getppid() returns 1 (the process ID of init).

Note:
Some implementations no longer re-parent orphaned processes directly
to `init`. There is a note about this in the TLPI Errata.
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    switch(fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        sleep(3); /* Wait for parent to terminate */
        printf("getppid() = %ld\n", (long) getppid());
        _exit(EXIT_SUCCESS);
    default:
        exit(EXIT_SUCCESS);
    }
}
