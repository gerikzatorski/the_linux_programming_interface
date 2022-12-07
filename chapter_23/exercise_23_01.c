/* exercise_23_01.c */

/*********************************************************************
Although alarm() is implemented as a system call within the Linux
kernel, this is redundant. Implement alarm() using setitimer().
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

unsigned int alarmcopy(unsigned int seconds)
{
    struct itimerval old_itv, new_itv;
    
    new_itv.it_value.tv_sec = (time_t) seconds;
    new_itv.it_value.tv_usec = 0;

    new_itv.it_interval.tv_sec = 0;
    new_itv.it_interval.tv_usec = 0;
    
    if (setitimer(ITIMER_REAL, &new_itv, &old_itv) == -1) {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }

    /* Truncates usec in return value */
    
    return old_itv.it_value.tv_sec;
}

static void handler(int sig)
{
    printf("ALARM!\n");
}

int main(int argc, char *argv[])
{
    time_t s;
    struct sigaction act;
    
    /* Parse commandline input */
    
    if (argc == 1) {
        s = 0;
    }
    else if (argc == 2) {
        s = (time_t) atoi(argv[1]);
    }
    else {
        fprintf(stderr, "Usage: %s [seconds]", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Setup SIGALRM handle */

    act.sa_handler = handler;
    
    if (sigaction(SIGALRM, &act, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    
    alarmcopy(s);

    if (s != 0) {
        pause();
    }
    
    exit(EXIT_SUCCESS);
}
