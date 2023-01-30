/* exercise_37_01.c */

/*********************************************************************
Write a program (similar to logger(1)) that uses syslog(3) to write
arbitrary messages to the system log file. As well as accepting a
single command-line argument containing the message to be logged, the
program should permit an option to specify the level of the message.
*********************************************************************/

#include <syslog.h>
#include "tlpi_hdr.h"

#define MSG_MAX 1024

int main(int argc, char *argv[])
{
  int opt, i, level = 0;
  char msg[MSG_MAX];
  
  while ((opt = getopt(argc, argv, ":l:")) != -1) {
    switch(opt) {
    case 'l':
      level = getInt(optarg, GN_NONNEG, "level");
      break;
    case '?':
      fprintf(stderr, "Unkown option: -%c\n", optopt);
      exit(EXIT_FAILURE);
    case ':':
      fprintf(stderr, "Missing arg for %c\n", optopt);
      exit(EXIT_FAILURE);
    }
  }

  if (!argv[optind])
    usageErr("%s [-l level] message\n", argv[0]);
  
  strcpy(msg, argv[optind++]);
  while (optind < argc) {
    strcat(msg, " ");
    strcat(msg, argv[optind++]);
  }

  /* openlog(); */
  syslog(LOG_USER | level, "%s", msg);
  closelog();

  exit(EXIT_SUCCESS);
}
