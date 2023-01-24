/* exercise_34_06.c */

/*********************************************************************
Write a program to verify that when a process in an orphaned process
group attempts to read() from the controlling terminal, the read()
fails with the error EIO.
*********************************************************************/

#include "tlpi_hdr.h"

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
  char buf[BUF_SIZE];

  switch (fork()) {
  case -1:
    errExit("fork");

  case 0:

    /* Must wait for parent to exit to be an orphaned process */
    
    sleep(3);

    if (read(STDIN_FILENO, buf, BUF_SIZE) == -1)
      errExit("read");

    _exit(EXIT_SUCCESS);
    
  default:
    break;
  }
  
  exit(EXIT_SUCCESS);
}
