/* exercise_20_03a.c */

/*********************************************************************
Write programs that verify the effect of the SA_RESETHAND and
SA_NODEFER flags when establishing a signal handler with sigaction().

This one verifies SA_RESETHAND.
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static volatile sig_atomic_t visits = 0;

void handler(int sig, siginfo_t *info, void *context)
{
    visits++;
}

int main(int argc, char *argv[])
{
    struct sigaction act;

    act.sa_sigaction = &handler;
    act.sa_flags = SA_RESETHAND;

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Visited handler %u times.\n", visits);
    raise(SIGINT);
    printf("Visited handler %u times.\n", visits);
    raise(SIGINT); /* This should interrupt the process */
    printf("Visited handler %u times.\n", visits);

    exit(EXIT_SUCCESS);
}
