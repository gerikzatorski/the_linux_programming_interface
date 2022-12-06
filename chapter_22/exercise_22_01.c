/* exercise_22_01.c */

/*********************************************************************
Section 22.2 noted that if a stopped process that has established a
handler for and blocked SIGCONT is later resumed as a consequence of
receiving a SIGCONT , then the handler is invoked only when SIGCONT is
unblocked. Write a program to verify this. Recall that a process can
be stopped by typing the terminal suspend character (usually
Control-Z) and can be sent a SIGCONT signal using the command kill
–CONT (or implicitly, using the shell fg command).
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t visited = 0;

void handler(int sig, siginfo_t *info, void *context)
{
    visited = 1;
}

int main(int argc, char *argv[])
{
    sigset_t set;
    struct sigaction act;

    act.sa_sigaction = &handler;

    if (sigaction(SIGCONT, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&set);
    sigaddset(&set, SIGCONT);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    /* Give user time to stop and continue process */
    
    sleep(10);

    if (visited) {
        printf("Handler was visited\n");
    }
    else {
        printf("Handler has NOT been visited\n");
    }
    
    sigemptyset(&set);
    sigaddset(&set, SIGCONT);

    if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
