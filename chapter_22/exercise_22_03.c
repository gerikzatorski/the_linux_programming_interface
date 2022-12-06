/* exercise_22_03.c */

/*********************************************************************
Section 22.10 stated that accepting signals using sigwaitinfo() is
faster than the use of a signal handler plus sigsuspend(). The program
signals/sig_speed_sigsuspend.c, supplied in the source code
distribution for this book, uses sigsuspend() to alternately send
signals back and forward between a parent and a child process. Time
the operation of this program to exchange one million signals between
the two processes. (The number of signals to exchange is provided as a
command-line argument to the program.) Create a modified version of
the program that instead uses sigwaitinfo(), and time that version.
What is the speed difference between the two programs?
*********************************************************************/

#include <signal.h>
#include "tlpi_hdr.h"

static void
handler(int sig)
{
}

#define TESTSIG SIGUSR1

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-sigs\n", argv[0]);

    int numSigs = getInt(argv[1], GN_GT_0, "num-sigs");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(TESTSIG, &sa, NULL) == -1)
        errExit("sigaction");

    /* Block the signal before fork(), so that the child doesn't manage
       to send it to the parent before the parent is ready to catch it */

    sigset_t blockedMask, emptyMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, TESTSIG);
    if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&emptyMask);

    pid_t childPid = fork();
    switch (childPid) {
    case -1: errExit("fork");

    case 0:     /* child */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (kill(getppid(), TESTSIG) == -1)
                errExit("kill");
            if (sigwaitinfo(&blockedMask, NULL) == -1) {
                errExit("sigwaitinfo");
            }
            /* if (sigsuspend(&emptyMask) == -1 && errno != EINTR) */
            /*         errExit("sigsuspend"); */
        }
        exit(EXIT_SUCCESS);

    default: /* parent */
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (sigwaitinfo(&blockedMask, NULL) == -1) {
                errExit("sigwaitinfo");
            }
            /* if (sigsuspend(&emptyMask) == -1 && errno != EINTR) */
            /*         errExit("sigsuspend"); */
            if (kill(childPid, TESTSIG) == -1)
                errExit("kill");
        }
        exit(EXIT_SUCCESS);
    }
}
