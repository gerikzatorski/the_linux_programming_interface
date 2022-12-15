/* exercise_26_04.c */

/*********************************************************************
Listing 26-4 ( make_zombie.c ) uses a call to sleep() to allow the
child process a chance to execute and terminate before the parent
executes system(). This approach produces a theoretical race
condition. Modify the program to eliminate the race condition by using
signals to synchronize the parent and child.
*********************************************************************/

#include <signal.h>
#include <libgen.h>             /* For basename() declaration */
#include "tlpi_hdr.h"

#define CMD_SIZE 200

static pid_t childPid;
static char progName[CMD_SIZE - 10];

static void handler(int sig)
{
    char cmd[CMD_SIZE];

    snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(progName));
    system(cmd);            /* View zombie child */

    /* Now send the "sure kill" signal to the zombie */

    if (kill(childPid, SIGKILL) == -1)
        errMsg("kill");
    sleep(3);               /* Give child a chance to react to signal */
    printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
    system(cmd);            /* View zombie child again */
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    
    setbuf(stdout, NULL);

    strcpy(progName, argv[0]);
    
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        errExit("sigaction");
 
    printf("Parent PID=%ld\n", (long) getpid());
    
    switch (childPid = fork()) {
    case -1:
        errExit("fork");

    case 0:     /* Child: immediately exits to become zombie */
        printf("Child (PID=%ld) exiting\n", (long) getpid());
        _exit(EXIT_SUCCESS);

    default:    /* Parent: loops waiting to handle SIGCHLD */
        for (;;) {
        }
    }
}
