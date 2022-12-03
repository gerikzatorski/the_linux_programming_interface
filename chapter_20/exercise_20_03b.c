/* exercise_20_03b.c */

/*********************************************************************
Write programs that verify the effect of the SA_RESETHAND and
SA_NODEFER flags when establishing a signal handler with sigaction().

This one verifies SA_NODEFER.
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>

static volatile sig_atomic_t is_deferred = 0;
static volatile sig_atomic_t executing_outer = 0;

void inner_handler(int sig, siginfo_t *info, void *context)
{
    if (!executing_outer) {
        is_deferred = 1;
    }
}

void outer_handler(int sig, siginfo_t *info, void *context)
{
    struct sigaction act;

    executing_outer = 1;
    act.sa_sigaction = &inner_handler;

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("sigaction");
        _exit(EXIT_FAILURE);
    }
    
    raise(SIGINT);
    executing_outer = 0;
}

int main(int argc, char *argv[])
{
    struct sigaction act;

    act.sa_sigaction = &outer_handler;
    act.sa_flags = SA_NODEFER;

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    raise(SIGINT);

    assert(!is_deferred);

    exit(EXIT_SUCCESS);
}
