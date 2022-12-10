/* exercise_25_01.c */

/*********************************************************************
If a child process makes the call exit(–1), what exit status (as
returned by WEXITSTATUS() ) will be seen by the parent?
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    
    switch(fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        _exit(-1);
    default:
        break;
    }

    wait(&status);
    printf("Child sent status of %d\n", WEXITSTATUS(status));

    exit(EXIT_SUCCESS);
}
