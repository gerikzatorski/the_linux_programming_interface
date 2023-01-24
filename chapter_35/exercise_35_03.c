/* exercise_35_03.c */

/*********************************************************************
Write a program that places itself under the SCHED_FIFO scheduling
policy and then creates a child process. Both processes should execute
a function that causes the process to consume a maximum of 3 seconds
of CPU time. (This can be done by using a loop in which the times()
system call is repeatedly called to determine the amount of CPU time
so far consumed.) After each quarter of a second of consumed CPU time,
the function should print a message that displays the process ID and
the amount of CPU time so far consumed. After each second of consumed
CPU time, the function should call sched_yield() to yield the CPU to
the other process. (Alternatively, the processes could raise each
other’s scheduling priority using sched_setparam().) The program’s
output should demonstrate that the two processes alternately execute
for 1 second of CPU time. (Note carefully the advice given in Section
35.3.2 about preventing a runaway realtime process from hogging the
CPU.)
*********************************************************************/

#define _GNU_SOURCE
#include <sched.h>
#include <sys/times.h>
#include "tlpi_hdr.h"

#define PRIO_DEFAULT 50
#define CPU_DEFAULT 0

static void track_time(void)
{
  struct tms t;
  clock_t t_start, t_curr, mark_quarter, mark_full;
  int ticks_per_second;

  ticks_per_second = sysconf(_SC_CLK_TCK);
  if (ticks_per_second == -1)
    errExit("sysconf");
       
 if (times(&t) == -1)
    errExit("times");
 
  t_start  = t.tms_utime;
  mark_quarter = t_start;
  mark_full = t_start;

  while(true) {
    if (times(&t) == -1)
      errExit("times");

    t_curr  = t.tms_utime;

    if ((double) (t_curr - t_start) / ticks_per_second > 3.0)
      break;

    if ((double) (t_curr - mark_quarter) / ticks_per_second > 0.25) {
      mark_quarter = t_curr;
      printf("[PID=%ld] %.4f seconds\n",
             (long) getpid(),
             (double) (t_curr - t_start) / ticks_per_second);
    }

    if ((double) (t_curr - mark_full) / ticks_per_second > 1.0) {
      mark_full = t_curr;
      sched_yield();
    }
  }
}

int main(int argc, char *argv[])
{
  struct sched_param sp;
  cpu_set_t set;

  /* Child process will inherit privileged scheduling policy */
  
  sp.sched_priority = PRIO_DEFAULT;
  if (sched_setscheduler(0, SCHED_FIFO, &sp) == -1)
    errExit("sched_setscheduler");

  /* Multiprocessor systems need CPU affinity set */
  /* CPU afffinity is inherited by child processes */
  
  CPU_ZERO(&set);
  CPU_SET(CPU_DEFAULT, &set);
  if (sched_setaffinity(0, CPU_SETSIZE, &set) == -1)
    errExit("sched_setaffinity");

  switch(fork()) {
  case -1:
    errExit("fork");

  case 0:
    track_time();
    _exit(EXIT_SUCCESS);

  default:
    break;
  }

  track_time();
  exit(EXIT_SUCCESS);
}
