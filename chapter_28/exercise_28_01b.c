/* exercise_28_01a.b */

/*********************************************************************
Write a program to see how fast the fork() and vfork() system calls
are on your system. Each child process should immediately exit, and
the parent should wait() on each child before creating the next.
Compare the relative differences for these two system calls with those
of Table 28-3. The shell built-in command time can be used to measure
the execution time of a program.

This program uses vfork().
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define DEFAULT_FORKS 1000000

int main(int argc, char *argv[])
{
    int num_forks, status, i;

    if (argc == 2) {
        num_forks = atoi(argv[1]);
    }
    else {
        num_forks = DEFAULT_FORKS;
    }

    for (i = 0; i < num_forks; i++) {
        switch(vfork()) {
        case -1:
            perror("vfork");
            exit(EXIT_FAILURE);
        case 0:
            _exit(EXIT_SUCCESS);
        default:
            wait(&status);
        }
    }
    
    exit(EXIT_SUCCESS);
}
