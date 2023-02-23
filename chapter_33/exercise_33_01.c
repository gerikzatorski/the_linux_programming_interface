/* exercise_33_01.c */

/*********************************************************************
Write a program to demonstrate that different threads in the same
process can have different sets of pending signals, as returned by
sigpending(). You can do this by using pthread_kill() to send
different signals to two different threads that have blocked these
signals, and then have each of the threads call sigpending() and
display information about pending signals. (You may find the functions
in Listing 20-4 useful.)
*********************************************************************/

#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void *thread_func(void *arg)
{
  int sig, s;
  sigset_t wait_set, pending_set;

  /* Sychronization: wait for SIGUSR1 */
  
  sigemptyset(&wait_set);
  sigaddset(&wait_set, SIGUSR1);

  s = sigwait(&wait_set , &sig);
  if (s != 0)
    errExitEN(s, "sigwait");

  /* Print pending signals */
  
  if (sigpending(&pending_set) == -1)
    errExit("sigpending");

  for (sig = 1; sig < NSIG; sig++)
    if (sigismember(&pending_set, sig))
      printf("Pending in thread %lu:\t%d (%s)\n", pthread_self(), sig, strsignal(sig));

  return NULL;
}

int main(int argc, char *argv[])
{
  sigset_t block_set;
  struct sigaction sa;
  int s;
  pthread_t th1, th2;

  /* Block signals (threads will inherit sigmask) */

  sigemptyset(&block_set);
  sigaddset(&block_set, SIGALRM);
  sigaddset(&block_set, SIGILL);
  sigaddset(&block_set, SIGUSR1);
  if (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1)
    errExit("sigprocmask");

  /* Create threads */
  
  s = pthread_create(&th1, NULL, thread_func, NULL);
  if (s != 0)
    errExitEN(s, "pthread_create");
  s = pthread_create(&th2, NULL, thread_func, NULL);
  if (s != 0)
    errExitEN(s, "pthread_create");

  /* Send different signals to threads */
  
  s = pthread_kill(th1, SIGALRM);
  if (s != 0)
    errExitEN(s, "pthread_kill");
  s = pthread_kill(th2, SIGILL);
  if (s != 0)
    errExitEN(s, "pthread_kill");

  /* Sychronization: send SIGUSR1 to each thread */

  s = pthread_kill(th1, SIGUSR1);
  if (s != 0)
    errExitEN(s, "pthread_kill");
  s = pthread_kill(th2, SIGUSR1);
  if (s != 0)
    errExitEN(s, "pthread_kill");

  /* Join the threads */
  
  s = pthread_join(th1, NULL);
  if (s != 0)
    errExitEN(s, "pthread_join");
  s = pthread_join(th2, NULL);
  if (s != 0)
    errExitEN(s, "pthread_join");
  
  exit(EXIT_SUCCESS);
}
