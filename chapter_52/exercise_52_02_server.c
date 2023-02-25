/* exercise_52_02_server.c */

/*********************************************************************
Recode the sequence-number client-server application of Section 44.8
to use POSIX message queues.
*********************************************************************/

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "tlpi_hdr.h"
#include "exercise_52_02.h"

int main(int argc, char *argv[])
{
    int server_flags, seq_num = 0;
    mode_t server_perms;
    mqd_t server_mqd, client_mqd;
    char client_name[CLIENT_MQ_NAME_LEN];
    ssize_t num_read;
    struct mq_attr server_attr;
    struct request req;
    struct response resp;

    /* Cleanup previous mq servers */
    
    if (mq_unlink(SERVER_MQ) == -1)
        errMsg("mq_unlink failed on %s", SERVER_MQ);

    /* Create server MQ */

    umask(0);    
    server_flags = O_RDWR | O_CREAT | O_EXCL;
    server_perms = S_IRUSR | S_IWUSR;  /* rw------- */
    
    server_attr.mq_maxmsg = MAX_MSG_MQ;
    server_attr.mq_msgsize = sizeof(struct request);

    server_mqd = mq_open(SERVER_MQ, server_flags, server_perms, &server_attr);
    if (server_mqd == (mqd_t) -1)
        errExit("mq_open");

    for (;;) {
        num_read = mq_receive(server_mqd, (char *) &req, sizeof(struct request), NULL);
        if (num_read == -1)
            errExit("mq_receive");

        /* Open client MQ (previously created by client) */

        snprintf(client_name, CLIENT_MQ_NAME_LEN, CLIENT_MQ_TEMPLATE,
                 (long) req.pid);
        client_mqd = mq_open(client_name, O_WRONLY);
        if (client_mqd == (mqd_t) -1) {
            errMsg("open %s", client_name);
            continue;
        }

        /* Send response and close MQ */

        resp.seq_num = seq_num;
        if (mq_send(client_mqd, (char *) &resp, sizeof(struct response), 0) == -1)
            fprintf(stderr, "Error writing to MQ %s\n", client_name);

        if (mq_close(client_mqd) == -1)
            errMsg("close");

        seq_num += req.seq_len;
    }
    
    exit(EXIT_SUCCESS);
}
