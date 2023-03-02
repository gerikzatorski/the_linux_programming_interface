/* exercise_57_01_server.c */

/*********************************************************************
In Section 57.3, we noted that UNIX domain datagram sockets are
reliable. Write programs to show that if a sender transmits datagrams
to a UNIX domain datagram socket faster than the receiver reads them,
then the sender is eventually blocked, and remains blocked until the
receiver reads some of the pending datagrams.
*********************************************************************/

#include <time.h>
#include "exercise_57_01.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un svaddr, claddr;
    int sfd, j, read_delay;
    ssize_t num_bytes;
    socklen_t len;
    char buf[BUF_SIZE];
    struct timespec ts;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s read-delay\n", argv[0]);

    ts.tv_sec = 0;
    ts.tv_nsec = getInt(argv[1], GN_GT_0, "read-delay");

    /* Create server socket */
    
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* Construct well-known address and bind server socket to it */

    /* For an explanation of the following check, see the erratum note for
       page 1168 at http://www.man7.org/tlpi/errata/. */

    if (strlen(SV_SOCK_PATH) > sizeof(svaddr.sun_path) - 1)
        fatal("Server socket path too long: %s", SV_SOCK_PATH);

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
        errExit("remove-%s", SV_SOCK_PATH);

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    /* Receive messages with delays in-between  */

    for (;;) {
        len = sizeof(struct sockaddr_un);
        num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0,
                             (struct sockaddr *) &claddr, &len);
        if (num_bytes == -1)
            errExit("recvfrom");

        buf[num_bytes] = '\0';
        printf("Server received: %s\n", buf);

        nanosleep(&ts, NULL); /* Delay to try and block client */
    }
}
