/* exercise_39_01.c */

/*********************************************************************
Modify the program in Listing 35-2 ( sched_set.c , on page 743) to use
file capabilities, so that it can be used by an unprivileged user.
*********************************************************************/

#include <sched.h>
#include <sys/capability.h>
#include "tlpi_hdr.h"

/* See Listing 39-1 */

static int modifyCap(int capability, int setting)
{
    cap_t caps;
    cap_value_t capList[1];

    /* Retrieve caller's current capabilities */

    caps = cap_get_proc();
    if (caps == NULL)
        return -1;

    /* Change setting of 'capability' in the effective set of 'caps'. The
       third argument, 1, is the number of items in the array 'capList'. */

    capList[0] = capability;
    if (cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting) == -1) {
        cap_free(caps);
        return -1;
    }

    /* Push modified capability sets back to kernel, to change
       caller's capabilities */

    if (cap_set_proc(caps) == -1) {
        cap_free(caps);
        return -1;
    }

    /* Free the structure that was allocated by libcap */

    if (cap_free(caps) == -1)
        return -1;

    return 0;
}

/* See Listing 39-1 */

static int raiseCap(int capability)
{
    return modifyCap(capability, CAP_SET);
}

/* See Listing 39-1 */

static int dropAllCaps(void)
{
    cap_t empty;
    int s;

    empty = cap_init();
    if (empty == NULL)
        return -1;

    s = cap_set_proc(empty);

    if (cap_free(empty) == -1)
        return -1;

    return s;
}

int main(int argc, char *argv[])
{
    int j, pol;
    struct sched_param sp;

    if (argc < 3 || strchr("rfo"
#ifdef SCHED_BATCH              /* Linux-specific */
                "b"
#endif
#ifdef SCHED_IDLE               /* Linux-specific */
                "i"
#endif
                , argv[1][0]) == NULL)
        usageErr("%s policy priority [pid...]\n"
                "    policy is 'r' (RR), 'f' (FIFO), "
#ifdef SCHED_BATCH              /* Linux-specific */
                "'b' (BATCH), "
#endif
#ifdef SCHED_IDLE               /* Linux-specific */
                "'i' (IDLE), "
#endif
                "or 'o' (OTHER)\n",
                argv[0]);

    pol = (argv[1][0] == 'r') ? SCHED_RR :
                (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH              /* Linux-specific, since kernel 2.6.16 */
                (argv[1][0] == 'b') ? SCHED_BATCH :
#endif
#ifdef SCHED_IDLE               /* Linux-specific, since kernel 2.6.23 */
                (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
                SCHED_OTHER;

    sp.sched_priority = getInt(argv[2], 0, "priority");

    if (raiseCap(CAP_SYS_NICE) == -1)
        fatal("raiseCap() failed");

    for (j = 3; j < argc; j++)
        if (sched_setscheduler(getLong(argv[j], 0, "pid"), pol, &sp) == -1)
            errExit("sched_setscheduler");

    if (dropAllCaps() == -1)
        fatal("dropAllCaps() failed");

    exit(EXIT_SUCCESS);
}
