/* exercise_36_03.c */

/*********************************************************************
Write programs to determine what happens if a process’s consumption of
various resources already exceeds the soft limit specified in a call
to setrlimit().
*********************************************************************/

#include <sys/resource.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 1024
#define LIM_SIZE 10

int main(int argc, char *argv[])
{
  int fd, i;
  char buf[BUF_SIZE];
  struct rlimit rl;

  if (argc != 2)
    usageErr("%s filename\n", argv[0]);

  fd = open(argv[1],
            O_CREAT | O_WRONLY | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if (fd == -1)
    errExit("open");

  /* First write x's to file */
  
  for (i = 0; i < BUF_SIZE; i++)
    buf[i] = 'x';

  if (write(fd, buf, BUF_SIZE) == -1)
    errExit("write");

  rl.rlim_cur = LIM_SIZE;
  rl.rlim_max = LIM_SIZE;

  /* Then set rlimit */
  
  if (setrlimit(RLIMIT_FSIZE, &rl) == -1)
    errExit("setrlimit");
  
  if (close(fd) == -1)
    errExit("close");

  exit(EXIT_SUCCESS);
}
