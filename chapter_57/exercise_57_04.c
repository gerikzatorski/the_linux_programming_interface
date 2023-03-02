/* exercise_57_04.c */

/*********************************************************************
Suppose that we create two UNIX domain datagram sockets bound to the
paths /somepath/a and /somepath/b , and that we connect the socket
/somepath/a to /somepath/b. What happens if we create a third datagram
socket and try to send (sendto()) a datagram via that socket to
/somepath/a? Write a program to determine the answer. If you have
access to other UNIX systems, test the program on those systems to see
if the answer differs.
*********************************************************************/

#include <sys/un.h>
#include <sys/socket.h>
#include "tlpi_hdr.h"

#define SOCK_PATH_A "/tmp/a_sock"
#define SOCK_PATH_B "/tmp/b_sock"
#define SOCK_PATH_C "/tmp/b_sock"

int main(int argc, char *argv[])
{
    struct sockaddr_un addra, addrb, addrc;
    int afd, bfd, cfd, j;
    ssize_t num_bytes;
    socklen_t len;
    char *buf = "Test message";

    /* Setup datagram socket A */
    
    afd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (afd == -1)
        errExit("socket");

    if (strlen(SOCK_PATH_A) > sizeof(addra.sun_path) - 1)
        fatal("Server socket path too long: %s", SOCK_PATH_A);

    if (remove(SOCK_PATH_A) == -1 && errno != ENOENT)
        errExit("remove-%s", SOCK_PATH_A);

    memset(&addra, 0, sizeof(struct sockaddr_un));
    addra.sun_family = AF_UNIX;
    strncpy(addra.sun_path, SOCK_PATH_A, sizeof(addra.sun_path) - 1);

    if (bind(afd, (struct sockaddr *) &addra, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    /* Setup datagram socket B */
    
    bfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (bfd == -1)
        errExit("socket");

    if (strlen(SOCK_PATH_B) > sizeof(addrb.sun_path) - 1)
        fatal("Server socket path too long: %s", SOCK_PATH_B);

    if (remove(SOCK_PATH_B) == -1 && errno != ENOENT)
        errExit("remove-%s", SOCK_PATH_B);

    memset(&addrb, 0, sizeof(struct sockaddr_un));
    addrb.sun_family = AF_UNIX;
    strncpy(addrb.sun_path, SOCK_PATH_B, sizeof(addrb.sun_path) - 1);

    if (bind(bfd, (struct sockaddr *) &addrb, sizeof(struct sockaddr_un)) == -1)
        errExit("bind");

    /* Connect A and B (record address of B as peer of A) */
    
    if (connect(afd, (struct sockaddr *) &addrb,
                sizeof(struct sockaddr_un)) == -1)
        errExit("connect");

    /* Setup datagram socket C */
    
    cfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (cfd == -1)
        errExit("socket");

    if (strlen(SOCK_PATH_C) > sizeof(addrc.sun_path) - 1)
        fatal("Server socket path too long: %s", SOCK_PATH_C);

    if (remove(SOCK_PATH_C) == -1 && errno != ENOENT)
        errExit("remove-%s", SOCK_PATH_C);

    memset(&addrc, 0, sizeof(struct sockaddr_un));
    addrc.sun_family = AF_UNIX;
    strncpy(addrc.sun_path, SOCK_PATH_C, sizeof(addrc.sun_path) - 1);

    /* Send messages from C socket to A */
    
    if (sendto(cfd, buf, strlen(buf), 0, (struct sockaddr *) &addra,
               sizeof(struct sockaddr_un)) != strlen(buf))
        fatal("sendto");
}
