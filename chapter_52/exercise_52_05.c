/* exercise_52_05.c */

/*********************************************************************
Modify the program in Listing 52-6 ( mq_notify_sig.c ) to demonstrate
that message notification established by mq_notify() occurs just once.
This can be done by removing the mq_notify() call inside the for loop.
*********************************************************************/

#include <signal.h>
#include <mqueue.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define NOTIFY_SIG SIGUSR1

static void handler(int sig)
{
    /* Just interrupt sigsuspend() */
    printf("Signal is being handled\n");
}

int main(int argc, char *argv[])
{
    struct sigevent sev;
    mqd_t mqd;
    struct mq_attr attr;
    void *buffer;
    ssize_t num_read;
    sigset_t block_mask, empty_mask;
    struct sigaction sa;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s mq-name\n", argv[0]);

    mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
    if (mqd == (mqd_t) -1)
        errExit("mq_open");

    if (mq_getattr(mqd, &attr) == -1)
        errExit("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        errExit("malloc");

    sigemptyset(&block_mask);
    sigaddset(&block_mask, NOTIFY_SIG);
    if (sigprocmask(SIG_BLOCK, &block_mask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(NOTIFY_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = NOTIFY_SIG;
    if (mq_notify(mqd, &sev) == -1)
        errExit("mq_notify");

    sigemptyset(&empty_mask);

    for (;;) {
        sigsuspend(&empty_mask);

        /* Remove mq_notify() withing for loop */
        
        /* if (mq_notify(mqd, &sev) == -1) */
        /*     errExit("mq_notify"); */

        while ((num_read = mq_receive(mqd, buffer, attr.mq_msgsize, NULL)) >= 0)
            printf("Read %ld bytes\n", (long) num_read);

        if (errno != EAGAIN)
            errExit("mq_receive");
    }
}
