#ifndef EXERCISE_44_03_H
#define EXERCISE_44_03_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define SERVER_FIFO "/tmp/seqnum_sv"

#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"

#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

#define SEQUENCE_BACKUP "/tmp/seqnum_backup"
                                /* Backup file for sequence numbers */
#define SEQUENCE_START 0        /* Starting sequence value */

struct request {
    pid_t pid;
    int seq_len;
};

struct response {
    int seq_num;
};

#endif /* EXERCISE_44_03_H */
