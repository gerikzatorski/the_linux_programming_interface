/* exercise_22_02.c */

/*********************************************************************
If both a realtime and a standard signal are pending for a process,
SUSv3 leaves it unspecified which is delivered first. Write a program
that shows what Linux does in this case. (Have the program set up a
handler for all signals, block signals for a period time so that you
can send various signals to it, and then unblock all signals.)
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define NUM_STANDARD 4
#define NUM_REALTIME 6

static int standard_sigs[] = {SIGALRM, SIGBUS, SIGHUP, SIGILL};

void handler(int sig, siginfo_t *info, void *context)
{
    printf("%d\n", info->si_signo); /* UNSAFE */
}

int main(int argc, char *argv[])
{
    int sig, i;
    sigset_t set;
    struct sigaction act;
    union sigval sv;

    /* Block all signals */
    
    sigfillset(&set);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    /* Setup handler for standard signals */
    
    act.sa_sigaction = &handler;
    act.sa_flags = SA_SIGINFO;

    for (i = 0; i < NUM_STANDARD; i++) {
        if (sigaction(standard_sigs[i], &act, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
    }

    /* Setup handler for realtime signals */

    for (i = 0; i < NUM_REALTIME; i++) {
        if (sigaction(SIGRTMIN + i, &act, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
    }

    /* Raise standard signals  */

    for (i = 0; i < NUM_STANDARD; i++) {
        raise(standard_sigs[i]);
    }

    /* Send realtime signal */
    
    for (i = 0; i < 4; i++) {
        if (sigqueue(getpid(), SIGRTMIN + i, sv) == -1) {
            perror("sigqueue");
            exit(EXIT_FAILURE);
        }
    }
    
    /* Unblock all signals */
    
    if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
