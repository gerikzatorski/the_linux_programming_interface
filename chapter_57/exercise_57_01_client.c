/* exercise_57_01_client.c */

/*********************************************************************
In Section 57.3, we noted that UNIX domain datagram sockets are
reliable. Write programs to show that if a sender transmits datagrams
to a UNIX domain datagram socket faster than the receiver reads them,
then the sender is eventually blocked, and remains blocked until the
receiver reads some of the pending datagrams.
*********************************************************************/

#include "exercise_57_01.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr;
    int sfd, j, num_msgs;
    size_t msg_len;
    char req[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-msgs\n", argv[0]);

    num_msgs = getInt(argv[1], GN_GT_0, "num-msgs");

    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* Construct address of server */

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    /* Send numbered messages to server */

    for (j = 0; j < num_msgs; j++) {
        msg_len = snprintf(req, BUF_SIZE, "Message #%d", j);

        printf("Sending: %s\n", req);
        if (sendto(sfd, req, msg_len, 0, (struct sockaddr *) &svaddr,
                   sizeof(struct sockaddr_un)) != msg_len)
            fatal("sendto");
    }

    exit(EXIT_SUCCESS);
}
