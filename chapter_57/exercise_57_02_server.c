/* exercise_57_02_server.c */

/*********************************************************************
Rewrite the programs in Listing 57-3 ( us_xfr_sv.c ) and Listing 57-4
( us_xfr_cl.c ) to use the Linux-specific abstract socket namespace
(Section 57.6).
*********************************************************************/

#include "exercise_57_02.h"
#define BACKLOG 5

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t num_read;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* Create an abstract address, bind socket to it,
       and make this a listening socket */

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(&addr.sun_path[1], "xyz", sizeof(addr.sun_path) - 2);
    
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    if (listen(sfd, BACKLOG) == -1)
        errExit("listen");

    for (;;) {
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
            errExit("accept");

        while ((num_read = read(cfd, buf, BUF_SIZE)) > 0)
            if (write(STDOUT_FILENO, buf, num_read) != num_read)
                fatal("partial/failed write");

        if (num_read == -1)
            errExit("read");

        if (close(cfd) == -1)
            errMsg("close");
    }
}
