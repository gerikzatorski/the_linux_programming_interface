/* exercise_52_06.c */

/*********************************************************************
Replace the use of a signal handler in Listing 52-6 (mq_notify_sig.c)
with the use of sigwaitinfo(). Upon return from sigwaitinfo(), display
the values in the returned siginfo_t structure. How could the program
obtain the message queue descriptor in the siginfo_t structure
returned by sigwaitinfo()?
*********************************************************************/

#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define NOTIFY_SIG SIGUSR1

int main(int argc, char *argv[])
{
    struct sigevent sev;
    mqd_t mqd;
    struct mq_attr attr;
    void *buffer;
    ssize_t num_read;
    sigset_t block_mask, wait_mask;
    int sig;
    siginfo_t si;
    union sigval sval;
    
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s mq-name notification-value\n", argv[0]);

    mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
    if (mqd == (mqd_t) -1)
        errExit("mq_open");

    /* Read notification value from arguments */
    
    sval.sival_int = getInt(argv[2], GN_GT_0, "notification-value");

    if (mq_getattr(mqd, &attr) == -1)
        errExit("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        errExit("malloc");

    sigemptyset(&block_mask);
    sigaddset(&block_mask, NOTIFY_SIG);
    if (sigprocmask(SIG_BLOCK, &block_mask, NULL) == -1)
        errExit("sigprocmask");
    
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = NOTIFY_SIG;
    sev.sigev_value = sval;
    
    if (mq_notify(mqd, &sev) == -1)
        errExit("mq_notify");

    /* Create signal mask for sigwaitinfo */
    
    sigemptyset(&wait_mask);
    sigaddset(&wait_mask, NOTIFY_SIG);

    for (;;) {
        if (sigwaitinfo(&wait_mask, &si) == -1)
            errExit("sigwaitinfo");

        if (mq_notify(mqd, &sev) == -1)
            errExit("mq_notify");

        /* Display values returned via siginfo_t struct */

        printf("got signal: %d (%s)\n", sig, strsignal(sig));
        printf("    si_signo=%d, si_code=%d (%s), si_value=%d\n",
               si.si_signo, si.si_code,
               (si.si_code == SI_USER) ? "SI_USER" :
               (si.si_code == SI_QUEUE) ? "SI_QUEUE" : "other",
               si.si_value.sival_int);
        printf("    si_pid=%ld, si_uid=%ld\n",
               (long) si.si_pid, (long) si.si_uid);

        while ((num_read = mq_receive(mqd, buffer, attr.mq_msgsize, NULL)) >= 0)
            printf("Read %ld bytes\n", (long) num_read);

        if (errno != EAGAIN)
            errExit("mq_receive");
    }
}
