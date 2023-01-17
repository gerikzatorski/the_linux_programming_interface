/* exercise_30_01.c */

/*********************************************************************
Modify the program in Listing 30-1 ( thread_incr.c ) so that each loop
in the thread’s start function outputs the current value of glob and
some identifier that uniquely identifies the thread. The unique
identifier for the thread could be specified as an argument to the
pthread_create() call used to create the thread. For this program,
that would require changing the argument of the thread’s start
function to be a pointer to a structure containing the unique
identifier and a loop limit value. Run the program, redirecting output
to a file, and then inspect the file to see what happens to glob as
the kernel scheduler alternates execution between the two threads.
*********************************************************************/

#include <pthread.h>
#include "tlpi_hdr.h"

static volatile int glob = 0;   /* "volatile" prevents compiler optimizations
                                   of arithmetic operations on 'glob' */
static void *                   /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;

    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
        printf("[Thread %lu] glob = %d\n", pthread_self(), glob);
    }

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int loops, s;

    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0)
        errExitEN(s, "pthread_create");
    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
