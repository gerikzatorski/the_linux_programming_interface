/* exercise_26_02.c */

/*********************************************************************
Suppose that we have three processes related as grandparent, parent,
and child, and that the grandparent doesn’t immediately perform a
wait() after the parent exits, so that the parent becomes a zombie.
When do you expect the grandchild to be adopted by init (so that
getppid() in the grandchild returns 1): after the parent terminates or
after the grandparent does a wait()? Write a program to verify your
answer.
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
        switch(fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            /* Child: wait for a few seconds for parent to terminate */
            printf("Child pid is %d\n", getpid());
            sleep(3);
            printf("Child getppid = %ld\n", (long) getppid());
            _exit(EXIT_SUCCESS);
        default:
            break;
        }
        /* Parent: exits immediately to assume zombie role */
        printf("Parent pid is %d\n", (long) getpid());
        _exit(EXIT_SUCCESS);
    default:
        break;
    }
    /* Grandparent: doesn't wait for Parent */
    printf("Grandparent pid is %d\n", (long) getpid());

    for (;;) {
        /* Terminate process with signal */
    }
}
