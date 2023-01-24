/* exercise_34_07.c */

/*********************************************************************
Write a program to verify that if one of the signals SIGTTIN, SIGTTOU,
or SIGTSTP is sent to a member of an orphaned process group, then the
signal is discarded (i.e., has no effect) if it would stop the process
(i.e., the disposition is SIG_DFL ), but is delivered if a handler is
installed for the signal.
*********************************************************************/

#include <signal.h>
#include "tlpi_hdr.h"

#define SYNC_SIG SIGUSR1
#define JOB_SIG SIGTTIN

static void print_handler(int sig)
{
  printf("Caught signal %2d (%s)\n", sig, strsignal(sig));
}

static void empty_handler(int sig)
{
}

int main(int argc, char *argv[])
{
  pid_t grandchild_pid;
  sigset_t block_mask, empty_mask;
  struct sigaction sa;

  setbuf(stdout, NULL); /* Disable buffering of stdout */

  /* Block signal */

  sigemptyset(&block_mask);
  sigaddset(&block_mask, SYNC_SIG);
  if (sigprocmask(SIG_BLOCK, &block_mask, NULL) == -1)
    errExit("sigprocmask");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = empty_handler;
  if (sigaction(SYNC_SIG, &sa, NULL) == -1)
    errExit("sigaction");

  sigemptyset(&empty_mask);

  switch (fork()) {
  case -1:
    errExit("fork");

  case 0:
    sleep(3); /* Must wait for parent to exit */
    
    switch (grandchild_pid = fork()) {
    case -1:
      errExit("fork");

    case 0:
      kill(getppid(), JOB_SIG);
      kill(getppid(), SYNC_SIG);

      /* Wait for child to setup handler */

      if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
        errExit("sigsuspend");

      kill(getppid(), JOB_SIG);

      _exit(EXIT_SUCCESS);

    default:
      break;
    }

    /* Wait for grandchild to send first job signal  */

    if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
      errExit("sigsuspend");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = print_handler;
    if (sigaction(JOB_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    if (kill(grandchild_pid, SYNC_SIG) == -1)
      errExit("kill");

    /* Wait for grandchild to send second job signal  */
    
    if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
      errExit("sigsuspend");
    
    _exit(EXIT_SUCCESS);

  default:
    break;
  }

  exit(EXIT_SUCCESS);
}
