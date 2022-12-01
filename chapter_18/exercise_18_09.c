/* exercise_18_09.c */

/*********************************************************************
In Section 18.10, we showed two different techniques (using fchdir()
and chdir(), respectively) to return to the previous current working
directory after changing the current working directory to another
location. Suppose we are performing such an operation repeatedly.
Which method do you expect to be more efficient? Why? Write a program
to confirm your answer.
 *********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

#define NUM_DIR_CHANGES 10000
#define TARGET_DIR "/"

double test_chdir(const int n)
{
    int i;
    clock_t start, end;
    char working_dir[PATH_MAX + 1];

    getcwd(working_dir, PATH_MAX);

    start = clock();
    for (i = 0; i < n; i++) {
        chdir(TARGET_DIR);
        chdir(working_dir);
    }
    end = clock();

    return (double)(end - start) / CLOCKS_PER_SEC;
}

double test_fchdir(const int n)
{
    int i, working_fd, target_fd;
    clock_t start, end;
    char working_dir[PATH_MAX + 1];

    getcwd(working_dir, PATH_MAX);
    target_fd = open(TARGET_DIR, O_RDONLY);
    working_fd = open(working_dir, O_RDONLY);
    
    start = clock();
    for (i = 0; i < n; i++) {
        fchdir(target_fd);
        fchdir(working_fd);
    }
    end = clock();

    close(working_fd);
    close(target_fd);
    
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(int argc, char *argv[])
{

    printf("chdir * %u = %f seconds\n", NUM_DIR_CHANGES,
           test_chdir(NUM_DIR_CHANGES));
    printf("fchdir * %u = %f seconds\n", NUM_DIR_CHANGES,
           test_fchdir(NUM_DIR_CHANGES));
    exit(EXIT_SUCCESS);
}
