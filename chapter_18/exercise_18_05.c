/* exercise_18_05.c */

/*********************************************************************
Implement a function that performs the equivalent of getcwd(). A
useful tip for solving this problem is that you can find the name of
the current working directory by using opendir() and readdir() to walk
through each of the entries in the parent directory (..) to find an
entry with the same i-node and device number as the current working
directory (i.e., respectively, the st_ino and st_dev fields in the
stat structure returned by stat() and lstat()). Thus, it is possible
to construct the directory path by walking up the directory tree
(chdir(“..”)) one step at a time and performing such scans. The walk
can be finished when the parent directory is the same as the current
working directory (recall that /.. is the same as /). The caller
should be left in the same directory in which it started, regardless
of whether your getcwd() function succeeds or fails (open() plus
fchdir() are handy for this purpose).
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

char *getcwdcopy(char *cwdbuf)
{
    DIR *dirp;
    struct dirent *dp;
    struct stat curr_sb, parent_sb;
    ino_t curr_ino;
    dev_t curr_dev;
    char temp[PATH_MAX - NAME_MAX - 1];
    char cwdstr[PATH_MAX + 1];

    for (;;) {

        if (stat(".", &curr_sb) == -1) {
            return NULL;
        }

        curr_ino = curr_sb.st_ino;
        curr_dev = curr_sb.st_dev;

        /* Finish walk when parent dir equals current dir */
        
        if (stat("..", &parent_sb) == -1) {
            return NULL;
        }
        
        if (parent_sb.st_ino == curr_ino) {
            break;
        }
        
        /* Scan parent directory to determine name */
        
        dirp = opendir("..");
        if (dirp == NULL) {
            fprintf(stderr, "opendir failed");
            return NULL;
        }

        while (1) {
            errno = 0;
            dp = readdir(dirp);
            if (dp == NULL) {
                break;
            }

            /* Only comparing i-node (not device number) */
            
            if (dp->d_fileno == curr_ino) {
                strcpy(temp, cwdstr);
                sprintf(cwdstr, "/%s%s", dp->d_name, temp);
            }
        }

        chdir("..");
    }

    strcpy(cwdbuf, cwdstr);
    return cwdbuf;
}

int main(int argc, char *argv[])
{
    char buf[PATH_MAX + 1];
    getcwdcopy(buf);
    printf("%s\n", buf);
    exit(EXIT_SUCCESS);
}
