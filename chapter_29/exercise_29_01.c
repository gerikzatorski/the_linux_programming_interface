/* exercise_29_01.c */

/*********************************************************************
What possible outcomes might there be if a thread executes the
following code:

pthread_join(pthread_self(), NULL);

Write a program to see what actually happens on Linux. If we have a
variable, tid, containing a thread ID, how can a thread prevent itself
from making a call, pthread_join(tid, NULL), that is equivalent to the
above statement?
*********************************************************************/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void thread_errno(int errnum, const char *funcname)
{
    fprintf(stderr, "%s: %s\n", funcname, strerror(errnum));
}

int main(int argc, char *argv[])
{
    void *res;
    int s;
    
    s = pthread_join(pthread_self(), &res);
    if (s != 0) {
        thread_errno(s, "pthread_create");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
