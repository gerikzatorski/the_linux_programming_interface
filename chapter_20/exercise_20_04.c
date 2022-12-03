/* exercise_20_04.c */

/*********************************************************************
Implement siginterrupt() using sigaction().
*********************************************************************/

#include <stdlib.h>
#include <signal.h>

int siginterruptcopy(int sig, int flag)
{
    struct sigaction old_act;

    /* Fetch a copy of the current disposition */
    
    if (sigaction(sig, NULL, &old_act) == -1) {
        return -1;
    }

    /* Tweak the SA_RESTART flag in returned structure */

    if (flag) {
        old_act.sa_flags &= ~SA_RESTART; /* Remove from bitmask */
    } else {
        old_act.sa_flags |= SA_RESTART;  /* Add to bitmask */
    }

    /* Update the signals disposition */

    if (sigaction(sig, &old_act, NULL) == -1) {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    /* TODO: Create test */
    
    exit(EXIT_SUCCESS);
}
