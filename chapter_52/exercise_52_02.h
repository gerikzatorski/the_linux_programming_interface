#ifndef EXERCISE_52_02_H
#define EXERCISE_52_02_H


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#define SERVER_MQ "/seqnum_mq_sv"
#define CLIENT_MQ_TEMPLATE "/seqnum_mq_cl.%ld"
                                /* Template for building client MQ name */
#define CLIENT_MQ_NAME_LEN (sizeof(CLIENT_MQ_TEMPLATE) + 20)
                                /* Space required for client MQ pathname
                                  (+20 as a generous allowance for the PID) */
#define MAX_MSG_MQ 10
#define MSG_SIZE_MQ 2048

struct request {                /* Request (client --> server) */
    pid_t pid;                  /* PID of client */
    int seq_len;                /* Length of desired sequence */
};

struct response {               /* Response (server --> client) */
    int seq_num;                /* Start of sequence */
};

#endif /* EXERCISE_52_02_H */
