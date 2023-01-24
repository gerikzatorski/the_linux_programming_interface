/* exercise_35_01.c */

/*********************************************************************
Implement the nice(1) command.
*********************************************************************/

#include <sys/resource.h>
#include "tlpi_hdr.h"

#define DEFAULT_ADJUSTMENT 10

static int parse_option(const char *arg, int *val)
{
  if (arg[0] != '-')
    return -1;
  
  *val = atoi(arg + 1); /* Not very robust */
  return 0;
}

int main(int argc, char *argv[])
{
  int i;
  char cmd[sysconf(_SC_ARG_MAX)];
  int value, adjustment;

  errno = 0;
  value = getpriority(PRIO_PROCESS, 0);
  if (value == -1 && errno != 0)
    errExit("getpriority");
  
  if (argc == 1) {
    printf("%d\n", value);
    exit(EXIT_SUCCESS);
  }

  if (strcmp(argv[1], "--help") == 0)
    usageErr("%s [OPTION] [COMMAND [ARG]...]\n", argv[0]);

  i = 2;
  if (parse_option(argv[1], &adjustment) == -1) {
    adjustment = DEFAULT_ADJUSTMENT;
    i = 1;
  }

  while (i < argc) {
    strncat(cmd, argv[i], strlen(argv[i]));
    strcat(cmd, " ");
    i++;
  }

  if (setpriority(PRIO_PROCESS, 0, value + adjustment) == -1)
    errExit("setpriority");

  system(cmd);
  
  exit(EXIT_SUCCESS);
}
