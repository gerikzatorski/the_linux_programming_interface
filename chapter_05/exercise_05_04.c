/* exercise_05_04.c */

/*********************************************************************
Implement dup() and dup2() using fcntl() and, where necessary,
close(). (You may ignore the fact that dup2() and fcntl() return
different errno values for some error cases.) For dup2(), remember to
handle the special case where oldfd equals newfd. In this case, you
should check whether oldfd is valid, which can be done by, for
example, checking if fcntl(oldfd, F_GETFL) succeeds. If oldfd is not
valid, then the function should return –1 with errno set to EBADF.
*********************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int dupcopy(int oldfd)
{
    int newfd;

    newfd = fcntl(oldfd, F_DUPFD, 0);
    if (newfd == -1) {
        errno = EBADF;
        return -1;
    }
    return newfd;
}

int dup2copy(int oldfd, int newfd)
{
    if (oldfd == newfd) {  /* handle special case */
        if(fcntl(oldfd, F_GETFL) == -1) {
            errno = EBADF;
            return -1;
        }
        else {
            return newfd;
        }
    }

    /* Create newfd vacancy if necessary */
    
    if(fcntl(newfd, F_GETFL) != -1) {
        if(close(newfd) == -1) {
            perror("close using fd");
            return -1;
        }
    }

    /* Make duplicate of oldfd. */
    
    if(fcntl(oldfd, F_DUPFD, newfd) == -1) {
        perror("fcntl");
        return -1;
    }

    return newfd;
}
