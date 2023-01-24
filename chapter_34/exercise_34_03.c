/* exercise_34_03.c */

/*********************************************************************
Write a program to verify that a call to setsid() from a process group
leader fails.
*********************************************************************/

#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
  if (setsid() == -1)
    errExit("setside");
  
  exit(EXIT_SUCCESS);
}
