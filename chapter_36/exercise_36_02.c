/* exercise_36_02.c */

/*********************************************************************
Write a program that executes a command and then displays its resource
usage. This is analogous to what the time(1) command does. Thus, we
would use this program as follows:

  $ ./rusage command arg...
*********************************************************************/

#include <sys/resource.h>
#include "tlpi_hdr.h"

static void print_timeval(const char *pre, const struct timeval *t)
{
  printf("%s\t%ld seconds, %ld microseconds\n",
         pre, t->tv_sec, t->tv_usec);
}

int main(int argc, char *argv[])
{
  int i;
  struct rusage ru;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s command arg\n", argv[0]);

  switch (fork()) {
  case -1:
    errExit("fork");

  case 0:
    execvp(argv[1], &argv[1]);
    err_exit("execvp");

  default:
    wait(NULL);

    if (getrusage(RUSAGE_CHILDREN, &ru) == -1)
      errExit("getrusage");

    print_timeval("user", &ru.ru_utime);
    print_timeval("sys", &ru.ru_stime);
    
    exit(EXIT_SUCCESS);
  }
}
