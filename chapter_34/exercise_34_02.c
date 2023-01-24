/* exercise_34_02.c */

/*********************************************************************
Write a program to verify that a parent process can change the process
group ID of one of its children before the child performs an exec(),
but not afterward.
*********************************************************************/

#include <signal.h>
#include "tlpi_hdr.h"

#define SYNC_SIG SIGUSR1

static void handler(int sig)
{
}

int main(int argc, char *argv[])
{
  pid_t child_pid;
  sigset_t block_mask, orig_mask, empty_mask;
  struct sigaction sa;

  setbuf(stdout, NULL); /* Disable buffering of stdout */

  /* Block sync signal */

  sigemptyset(&block_mask);
  sigaddset(&block_mask, SYNC_SIG);
  if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1)
    errExit("sigprocmask");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = handler;
  if (sigaction(SYNC_SIG, &sa, NULL) == -1)
    errExit("sigaction");
  
  switch (child_pid = fork()) {
  case -1:
    errExit("fork");

  case 0:                       /* Child */

    /* Wait for parent to change process group ID */

    sigemptyset(&empty_mask);
    if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
      errExit("sigsuspend");

    printf("Child pgid \t%ld\n", (long) getpgrp());

    execlp("echo", "echo", "Child performing execlp", (char *) NULL);
    errExit("execlp echo");
    
    _exit(EXIT_SUCCESS);

  default:                      /* Parent */

    printf("Parent pgid \t%ld\n", (long) getpgrp());
    
    if (setpgid(child_pid, 0) == -1)
      errExit("setpgid");
    
    /* Signal child that process group id has been changed */
    
    if (kill(child_pid, SYNC_SIG) == -1)
      errExit("kill");

    sleep(3); /* Give time for child to exec() */

    if (setpgid(child_pid, 0) == -1)
      errExit("setpgid");

    /* Should not reach this point */

    exit(EXIT_SUCCESS);
  }
  
  exit(EXIT_SUCCESS);
}
