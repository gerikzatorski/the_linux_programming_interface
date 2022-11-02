/* exercise_12_03.c */

/*********************************************************************
Write a program that lists all processes that have a particular file
pathname open. This can be achieved by inspecting the contents of all
of the /proc/PID/fd/* symbolic links. This will require nested loops
employing readdir(3) to scan all /proc/PID directories, and then the
contents of all /proc/PID/fd entries within each /proc/PID directory.
To read the contents of a /proc/PID/fd/n symbolic link requires the
use of readlink(), described in Section 18.5.
*********************************************************************/

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

int print_proc_if_file_open(pid_t pid, char *filename)
{
    DIR *dirp;
    struct dirent *dp;
    char pidpath[PATH_MAX];
    char sympath[PATH_MAX];
    int symfd;
    char destpath[PATH_MAX];
    ssize_t num_bytes;
    
    sprintf(pidpath, "/proc/%d/fd/", pid);

    dirp = opendir(pidpath);
    if (dirp == NULL) {
        return -1;
    }
    
    while (1) {
        errno = 0;
        dp = readdir(dirp);
        if (dp == NULL) {
            break;
        }

        symfd = atoi(dp->d_name);
        if (symfd == 0) {
            continue;  /* Skip non-numerical subdirs */
        }

        sprintf(sympath, "/proc/%d/fd/%d", pid, symfd);

        num_bytes = readlink(sympath, destpath, PATH_MAX - 1);
        if (num_bytes == -1) {
            exit(EXIT_FAILURE);
        }
        destpath[num_bytes] = '\0';  /* Add terminating null byte */

        if (strcmp(filename, destpath) == 0) {
            printf("%d\n", pid);
        }
    }
    
    if (errno != 0) {
        return -1;
    }
    
    if (closedir(dirp) == -1) {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    DIR *dirp;
    struct dirent *dp;
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    dirp = opendir("/proc/");
    if (dirp == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while (1) {
        errno = 0;
        dp = readdir(dirp);
        if (dp == NULL) {
            break;
        }

        pid = atoi(dp->d_name);
        if (pid == 0) {
            continue;  /* Skip non-numerical subdirs */
        }

        if (print_proc_if_file_open(pid, argv[1]) == -1) {
            perror("print_proc_if_file_open");
            exit(EXIT_FAILURE);
        }
    }
    
    if (errno != 0) {
        perror("readdir");
        exit(EXIT_FAILURE);
    }
    
    if (closedir(dirp) == -1) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
