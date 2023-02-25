/* exercise_52_01.c */

/*********************************************************************
Modify the program in Listing 52-5 ( pmsg_receive.c ) to accept a
timeout (a relative number of seconds) on the command line, and use
mq_timedreceive() instead of mq_receive().
*********************************************************************/

#include <mqueue.h>
#include <fcntl.h>              /* For definition of O_NONBLOCK */
#include <time.h>
#include "tlpi_hdr.h"

static void usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s [-n] [-t timeout] mq-name\n", progName);
    fprintf(stderr, "    -n           Use O_NONBLOCK flag\n");
    fprintf(stderr, "    -t           Timeout (seconds)\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int flags, opt;
    mqd_t mqd;
    unsigned int prio;
    void *buffer;
    struct mq_attr attr;
    ssize_t num_read;
    struct timespec timeout;
    int relative_timeout;
    
    flags = O_RDONLY;
    while ((opt = getopt(argc, argv, "nt:")) != -1) {
        switch (opt) {
        case 'n':
            flags |= O_NONBLOCK;
            break;
        case 't':
            relative_timeout = getInt(optarg, GN_GT_0, "relative-timeout");
            break;
        default:
            usageError(argv[0]);
        }
    }

    if (optind >= argc)
        usageError(argv[0]);
    
    mqd = mq_open(argv[optind], flags);
    if (mqd == (mqd_t) -1)
        errExit("mq_open");

    /* Specify timeout relative to Epoch */
    
    if (clock_gettime(CLOCK_REALTIME, &timeout) == -1)
        errExit("clock_gettime");

    timeout.tv_sec += relative_timeout;
    
    if (mq_getattr(mqd, &attr) == -1)
        errExit("mq_getattr");

    buffer = malloc(attr.mq_msgsize);
    if (buffer == NULL)
        errExit("malloc");

    num_read = mq_timedreceive(mqd, buffer, attr.mq_msgsize, &prio, &timeout);
    if (num_read == -1)
        errExit("mq_timedreceive");

    printf("Read %ld bytes; priority = %u\n", (long) num_read, prio);
    if (write(STDOUT_FILENO, buffer, num_read) == -1)
        errExit("write");
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}
