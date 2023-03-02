/* exercise_57_03_server.c */

/*********************************************************************
Reimplement the sequence-number server and client of Section 44.8
using UNIX domain stream sockets.
*********************************************************************/

#include "exercise_57_03.h"

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int sfd, cfd, seq_num = 0;
    ssize_t num_read;
    struct request req;
    struct response resp;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
        errExit("socket");

    /* Create an abstract server address, bind socket to it,
       and make this a listening socket */

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(&addr.sun_path[1], SERVER_SOCK, sizeof(addr.sun_path) - 2);
    
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    if (listen(sfd, BACKLOG) == -1)
        errExit("listen");

    for (;;) {

        /* Accept a connection on a new socket */
        
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
            errExit("accept");

        num_read = read(cfd, &req, sizeof(struct request));
        if (num_read == -1)
            errExit("read");

        /* Construct response, send it, and close socket */

        resp.seq_num = seq_num;
        
        if (write(cfd, &resp, sizeof(struct response)) == -1)
            errExit("write");

        if (close(cfd) == -1)
            errMsg("close");

        seq_num += req.seq_len;
    }
}
