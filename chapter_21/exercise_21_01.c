/* exercise_21_01.c */

/*********************************************************************
Implement abort().
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void abortcopy(void)
{
    sigset_t set;
    struct sigaction act;

    sigemptyset(&set);
    sigaddset(&set, SIGABRT);
    
    /* Override the effect of blocking  */
    
    if (sigprocmask(SIG_UNBLOCK, &set, &act) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    /* Get info on current handler */
    
    if (sigaction(SIGABRT, NULL, &act) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Raise once if not default */
    
    if (act.sa_handler != SIG_DFL) {
        raise(SIGABRT);
    }
    
    /* Reset handling of SIGABRT if reached here */

    act.sa_sigaction = SIG_DFL;

    if (sigaction(SIGABRT, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Flush stdio streams */

    if (fflush(stdout) == EOF || fflush(stderr) == EOF) {
        perror("fflush");
        exit(EXIT_FAILURE);
    }

    /* Close stdio streams */

    if (close(STDOUT_FILENO) == -1 || close(STDERR_FILENO) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    /* Second SIGABRT is guaranteed to kill the process */
    
    raise(SIGABRT);
}

void handler(int sig, siginfo_t *info, void *context)
{
    abortcopy();
}

int main(int argc, char *argv[])
{
    struct sigaction act;

    act.sa_sigaction = &handler;

    if (sigaction(SIGINT, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("This buffered output should still print.");
    pause();

    exit(EXIT_SUCCESS);
}
