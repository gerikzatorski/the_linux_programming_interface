/* exercise_27_06.c */

/*********************************************************************
Suppose that a parent process has established a handler for SIGCHLD
and also blocked this signal. Subsequently, one of its children exits,
and the parent then does a wait() to collect the child’s status. What
happens when the parent unblocks SIGCHLD ? Write a program to verify
your answer. What is the relevance of the result for a program calling
the system() function?
*********************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

static volatile sig_atomic_t entered = 0;

static void handler(int sig)
{
    entered = 1;
}

int main(int argc, char *argv[])
{
    sigset_t child_set;
    struct sigaction act;
    int status;
    
    act.sa_handler = handler;

    if (sigaction(SIGCHLD, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&child_set);
    sigaddset(&child_set, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &child_set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    
    switch(fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        _exit(EXIT_SUCCESS);
    default:
        break;
    }

    wait(&status);

    if (sigprocmask(SIG_UNBLOCK, &child_set, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    if (entered) {
        printf("The handler was invoked.\n");
    }
    else {
        printf("The handler was NOT invoked.\n");
    }
    
    exit(EXIT_SUCCESS);
}
