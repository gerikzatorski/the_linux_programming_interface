/* exercise_35_02.c */

/*********************************************************************
Write a set-user-ID-root program that is the realtime scheduling
analog of nice(1). The command-line interface of this program should
be as follows:

  # ./rtsched policy priority command arg...

In the above command, policy is r for SCHED_RR or f for SCHED_FIFO .
This program should drop its privileged ID before execing the command,
for the reasons described in Sections 9.7.1 and 38.3.
*********************************************************************/

#include <sched.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
  int i, pol;
  char cmd[sysconf(_SC_ARG_MAX)];
  struct sched_param sp;

  if (argc < 4 || strcmp(argv[1], "--help") == 0)
    usageErr("%s policy priority command arg...\n", argv[0]);

  pol = (argv[1][0] == 'r') ? SCHED_RR :
        (argv[1][0] == 'f') ? SCHED_FIFO :
        0;
  if (pol == 0)
    usageErr("%s policy priority command arg...\n", argv[0]);

  sp.sched_priority = getInt(argv[2], 0, "priority");

  for (i = 3; i < argc; i++) {
    strncat(cmd, argv[i], strlen(argv[i]));
    strcat(cmd, " ");
  }

  if (sched_setscheduler(0, pol, &sp) == -1)
    errExit("sched_setscheduler");

  /* Drop privileges for security */
  
  if (setuid(getuid()) == -1)
    errExit("setuid");

  system(cmd);
  
  exit(EXIT_SUCCESS);
}
