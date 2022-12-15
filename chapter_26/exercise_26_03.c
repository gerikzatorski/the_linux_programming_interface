/* exercise_26_03.c */

/*********************************************************************
Replace the use of waitpid() with waitid() in Listing 26-3 (
child_status.c ). The call to printWaitStatus() will need to be
replaced by code that prints relevant fields from the siginfo_t
structure returned by waitid().
*********************************************************************/

#include <sys/wait.h>
#include "tlpi_hdr.h"

void
printField(const char *msg, const siginfo_t *info)
{
    if (msg != NULL)
        printf("%s", msg);

    if (info->si_code == CLD_EXITED) {
        printf("child exited, status=%d\n", info->si_status);

    } else if (info->si_code == CLD_KILLED) {
        printf("child killed by signal %d (%s)\n",
                info->si_status, strsignal(info->si_status));

    } else if (info->si_code == CLD_STOPPED) {
        printf("child stopped by signal %d (%s)\n",
                info->si_status, strsignal(info->si_status));

#ifdef WIFCONTINUED
    } else if (info->si_code == CLD_CONTINUED) {
        printf("child continued\n");
#endif

    } else {
        printf("what happened to this child? (status=%x)\n",
                (unsigned int) info->si_status);
    }
}

int
main(int argc, char *argv[])
{
    int status;
    pid_t childPid;
    siginfo_t info;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [exit-status]\n", argv[0]);

    switch (fork()) {
    case -1: errExit("fork");

    case 0:             /* Child: either exits immediately with given
                           status or loops waiting for signals */
        printf("Child started with PID = %ld\n", (long) getpid());
        if (argc > 1)                   /* Status supplied on command line? */
            exit(getInt(argv[1], 0, "exit-status"));
        else                            /* Otherwise, wait for signals */
            for (;;)
                pause();
        exit(EXIT_FAILURE);             /* Not reached, but good practice */

    default:            /* Parent: repeatedly wait on child until it
                           either exits or is terminated by a signal */
        for (;;) {
            if (waitid(P_ALL, NULL, &info, WEXITED | WSTOPPED
#ifdef WCONTINUED       /* Not present on older versions of Linux */
                                                | WCONTINUED
#endif
                       ) == -1) {
                perror("waitid");
                exit(EXIT_FAILURE);
            }

            /* Print status in hex, and as separate decimal bytes */

            status = info.si_status;
            
            printf("waitid() returned: PID=%ld; status=0x%04x (%d,%d)\n",
                    (long) info.si_pid,
                    (unsigned int) info.si_status,
                    info.si_status >> 8,
                    info.si_status & 0xff);
            printField(NULL, &info);
            if (WIFEXITED(status) || WIFSIGNALED(status))
                exit(EXIT_SUCCESS);
        }
    }
}
