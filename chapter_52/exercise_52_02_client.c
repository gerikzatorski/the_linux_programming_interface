/* exercise_52_02_client.c */

/*********************************************************************
Recode the sequence-number client-server application of Section 44.8
to use POSIX message queues.
*********************************************************************/

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "tlpi_hdr.h"
#include "exercise_52_02.h"

static char client_name[CLIENT_MQ_NAME_LEN];

static void remove_mq(void) /* Invoked on exit to delete mq */
{
    mq_unlink(client_name);
}

int main(int argc, char *argv[])
{
    int client_flags;
    mode_t client_perms;
    mqd_t server_mqd, client_mqd;
    ssize_t num_read;
    struct mq_attr client_attr;
    struct request req;
    struct response resp;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-len]\n", argv[0]);

    /* Create client MQ (before sending request, to avoid a race) */

    umask(0);
    client_flags = O_RDWR | O_CREAT | O_EXCL;
    client_perms = S_IRUSR | S_IWUSR; /* rw------- */
    
    client_attr.mq_maxmsg = MAX_MSG_MQ;
    client_attr.mq_msgsize = sizeof(struct response);

    snprintf(client_name, CLIENT_MQ_NAME_LEN, CLIENT_MQ_TEMPLATE,
             (long) getpid());
    client_mqd = mq_open(client_name, client_flags,
                         client_perms, &client_attr);
    if (server_mqd == (mqd_t) -1)
        errExit("mq_open - client");

    if (atexit(remove_mq) != 0)
        errExit("atexit");

    /* Construct request message, open server MQ, and send message */

    req.pid = getpid();
    req.seq_len = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;

    server_mqd = mq_open(SERVER_MQ, O_RDWR);
    if (server_mqd == (mqd_t) -1)
        errExit("mq_open - server");

    if (mq_send(server_mqd, (char *) &req, sizeof(struct request), 0) == -1)
        fatal("Can't write to server");
    
    /* Read and display response */

    num_read = mq_receive(client_mqd, (char *) &resp, sizeof(struct response), NULL);
    if (num_read == -1)
        fatal("Can't read response from server");

    printf("%d\n", resp.seq_num);

    exit(EXIT_SUCCESS);
}
