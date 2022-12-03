/* exercise_20_02.c */

/*********************************************************************
Write a program that shows that when the disposition of a pending
signal is changed to be SIG_IGN , the program never sees (catches) the
signal.
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    sigset_t block_set, prev_mask;

    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    
    if (sigprocmask(SIG_BLOCK, &block_set, &prev_mask) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    raise(SIGINT);

    /* SIGINT is pending at this point */

    signal(SIGINT, SIG_IGN);
    
    if (sigprocmask(SIG_SETMASK, &prev_mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    printf("Successfully ignored SIGINT\n");

    exit(EXIT_SUCCESS);
}
