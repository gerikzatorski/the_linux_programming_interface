/* exercise_23_03.c */

/*********************************************************************
Write a program to show that if the evp argument to timer_create() is
specified as NULL , then this is equivalent to specifying evp as a
pointer to a sigevent structure with sigev_notify set to
SIGEV_SIGNAL, sigev_signo set to SIGALRM , and si_value.sival_int set
to the timer ID.
*********************************************************************/

#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_SECONDS 4

static void handler(int sig)
{
    printf("ALARM!\n");
}

int main(int argc, char *argv[])
{
    struct sigaction act;
    timer_t timer_id[2];
    struct itimerspec new_its;
    struct sigevent sev;

    /* Setup handler for SIGALRM */
    
    act.sa_handler = handler;
    
    if (sigaction(SIGALRM, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* With NULL */
    
    timer_create(CLOCK_REALTIME, NULL, &timer_id[0]);

    new_its.it_value.tv_sec = NUM_SECONDS;
    new_its.it_value.tv_nsec = 0;

    new_its.it_interval.tv_sec = 0;
    new_its.it_interval.tv_nsec = 0;

    timer_settime(timer_id[0], 0, &new_its, NULL);

    pause();
    
    /* Without NULL */

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_int = 1;
    
    timer_create(CLOCK_REALTIME, &sev, &timer_id[1]);
    
    timer_settime(timer_id[1], 0, &new_its, NULL);

    pause();
    
    exit(EXIT_SUCCESS);
}
