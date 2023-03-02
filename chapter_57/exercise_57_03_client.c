/* exercise_57_03_client.c */

/*********************************************************************
Reimplement the sequence-number server and client of Section 44.8
using UNIX domain stream sockets.
*********************************************************************/

#include "exercise_57_03.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd;
    ssize_t num_read;
    struct request req;
    struct response resp;
    
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [seq-len]\n", argv[0]);

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* Construct abstract server address, and make the connection */

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(&addr.sun_path[1],
            SERVER_SOCK, sizeof(addr.sun_path) - 2);

    if (connect(sfd, (struct sockaddr *) &addr,
                sizeof(struct sockaddr_un)) == -1)
        errExit("connect");

    /* Construct request and send message */

    req.seq_len = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;
    
    if (write(sfd, &req, sizeof(struct request)) == -1)
        errExit("write");

    /* Read and display response */
    
    num_read = read(sfd, &resp, sizeof(struct response));
    if (num_read == -1)
        errExit("read");

    printf("%d\n", resp.seq_num);

    exit(EXIT_SUCCESS);
}
