/* exercise_15_06.c */

/*********************************************************************
The chmod a+rX file command enables read permission for all categories
of user, and likewise enables execute permission for all categories of
user if file is a directory or execute permission is enabled for any
of the user categories for file, as demonstrated in the following
example:

$ ls -ld dir file prog
dr--------  2 mtk users    48 May  4 12:28 dir
-r--------  1 mtk users 19794 May  4 12:22 file
-r-x------  1 mtk users 19336 May  4 12:21 prog

$ chmod a+rX dir file prog
$ ls -ld dir file prog
dr-xr-xr-x  2 mtk users    48 May  4 12:28 dir
-r--r--r--  1 mtk users 19794 May  4 12:22 file
-r-xr-xr-x  1 mtk users 19336 May  4 12:21 prog

Write a program that uses stat() and chmod() to perform the equivalent
of chmod a+rX.
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>

int chmod_a_plus_rX(const char *pathname)
{
    struct stat sb;
    mode_t perm;

    if (stat(pathname, &sb) == -1) {
        return -1;
    }
    
    perm = sb.st_mode;

    if (S_ISDIR(sb.st_mode) ||
        (perm & S_IXUSR) ||
        (perm & S_IXGRP) ||
        (perm & S_IXOTH)) {

        /* A single chmod call wasn't working for execute perms */
        
        if (chmod(pathname, perm | S_IXUSR) == -1 ||
            chmod(pathname, perm | S_IXGRP) == -1 ||
            chmod(pathname, perm | S_IXOTH) == -1) {
            return -1;
        }
    }

    /* A single call for read permissions works */
    
    if (chmod(pathname, perm | S_IRUSR | S_IRGRP | S_IROTH) == -1) {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char pathname[PATH_MAX + 1];
    int i;
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++) {
        strcpy(pathname, argv[i]);
        if (chmod_a_plus_rX(pathname) == -1) {
            perror("chmod_a_plus_rX");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
