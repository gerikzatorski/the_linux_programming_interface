/* exercise_23_04.c */

/*********************************************************************
Modify the program in Listing 23-5 ( ptmr_sigev_signal.c ) to use
sigwaitinfo() instead of a signal handler.
*********************************************************************/

#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include "curr_time.h"                  /* Declares currTime() */
#include "itimerspec_from_str.h"        /* Declares itimerspecFromStr() */
#include "tlpi_hdr.h"

#define TIMER_SIG SIGRTMAX              /* Our timer notification signal */

static void
handler(int sig, siginfo_t *si, void *uc)
{
    /* Handler remains to prevent exit, get info from sigwaitinfo() */
}

int
main(int argc, char *argv[])
{
    int sig;
    struct itimerspec ts;
    struct sigaction  sa;
    struct sigevent   sev;
    timer_t *tidlist;
    int j;
    sigset_t set;
    siginfo_t info;
    
    if (argc < 2)
        usageErr("%s secs[/nsecs][:int-secs[/int-nsecs]]...\n", argv[0]);

    tidlist = calloc(argc - 1, sizeof(timer_t));
    if (tidlist == NULL)
        errExit("malloc");

    /* Establish handler for notification signal */

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(TIMER_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    /* Create and start one timer for each command-line argument */

    sev.sigev_notify = SIGEV_SIGNAL;    /* Notify via signal */
    sev.sigev_signo = TIMER_SIG;        /* Notify using this signal */

    for (j = 0; j < argc - 1; j++) {
        itimerspecFromStr(argv[j + 1], &ts);

        if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1)
            errExit("timer_create");
        printf("Timer ID: %ld (%s)\n", (long) tidlist[j], argv[j + 1]);

        if (timer_settime(tidlist[j], 0, &ts, NULL) == -1)
            errExit("timer_settime");
    }

    sigemptyset(&set);
    sigaddset(&set, TIMER_SIG);
    
    for (;;) {
        sig = sigwaitinfo(&set, &info);
        if (sig == -1)
            errExit("sigwaitinfo");
        
        printf("[%s] Got signal %d\n", currTime("%T"), info.si_signo);
        printf("    info.si_overrun = %d\n", info.si_overrun);
    }
}
