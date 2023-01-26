/* exercise_36_01.c */

/*********************************************************************
Write a program that shows that the getrusage() RUSAGE_CHILDREN flag
retrieves information about only the children for which a wait() call
has been performed. (Have the program create a child process that
consumes some CPU time, and then have the parent call getrusage()
before and after calling wait().)
*********************************************************************/

#include <sys/resource.h>
#include <signal.h>
#include "tlpi_hdr.h"

#define SYNC_SIG SIGUSR1

static void print_timeval(const char *pre, const struct timeval *t)
{
  printf("%s\t%ld seconds, %ld microseconds\n",
         pre, t->tv_sec, t->tv_usec);
}

static void handler(int sig)
{
}

int main(int argc, char *argv[])
{
  int i, j;
  sigset_t block_mask, empty_mask;
  struct sigaction sa;
  struct rusage ru;
  
  sigemptyset(&block_mask);
  sigaddset(&block_mask, SYNC_SIG);
  if (sigprocmask(SIG_BLOCK, &block_mask, NULL) == -1)
    errExit("sigprocmask");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sa.sa_handler = handler;
  if (sigaction(SYNC_SIG, &sa, NULL) == -1)
    errExit("sigaction");

  switch (fork()) {
  case -1:
    errExit("fork");

  case 0:
    for (i = 0, j = 0; i < 1000000; i++)
      j += 1;

    if (kill(getppid(), SYNC_SIG) == -1)
      errExit("kill");

    _exit(EXIT_SUCCESS);

  default:

    /* Wait for child to do some work */
    
    sigemptyset(&empty_mask);
    if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
      errExit("sigsuspend");

    if (getrusage(RUSAGE_CHILDREN, &ru) == -1)
      errExit("getrusage");

    print_timeval("user", &ru.ru_utime);
    print_timeval("sys", &ru.ru_stime);

    printf("Waiting for child...\n");
    wait(NULL);

    if (getrusage(RUSAGE_CHILDREN, &ru) == -1)
      errExit("getrusage");

    print_timeval("user", &ru.ru_utime);
    print_timeval("sys", &ru.ru_stime);

    exit(EXIT_SUCCESS);
  }
}
