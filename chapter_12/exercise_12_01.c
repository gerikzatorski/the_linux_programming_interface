/* exercise_12_01.c */

/*********************************************************************
Write a program that lists the process ID and command name for all
processes being run by the user named in the program’s command-line
argument. (You may find the userIdFromName() function from Listing
8-1, on page 159, useful.) This can be done by inspecting the Name:
and Uid: lines of all of the /proc/PID/status files on the system.
Walking through all of the /proc/PID directories on the system
requires the use of readdir(3), which is described in Section 18.8.
Make sure your program correctly handles the possibility that a
/proc/PID directory disappears between the time that the program
determines that the directory exists and the time that it tries to
open the corresponding /proc/PID/status file.
*********************************************************************/

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

#define MAX_LINE 128
#define MAX_CMD 64

/* Taken from Listing 8-1 */

uid_t uid_from_name(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0') {
        return -1;
    }
    
    u = strtol(name, &endptr, 10);
    if (*endptr == '\0') {
        return u;
    }

    pwd = getpwnam(name);
    if (pwd == NULL) {
        return -1;
    }

    return pwd->pw_uid;
}

void display_process_if_user(const pid_t pid, const uid_t uid) {
    char filepath[PATH_MAX + 1];
    FILE *fp;
    char *line = NULL;
    size_t num_read;
    uid_t uid_proc;
    pid_t pid_proc;
    char cmd_proc[MAX_CMD];
    
    sprintf(filepath, "/proc/%d/status", pid);

    fp = fopen(filepath, "r");
    if (fp == 0) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    /* Parse status files defensively */
    
    while (getline(&line, &num_read, fp) != -1) {
        if (strncmp(line, "Name", 4) == 0) {
            sscanf(line, "%*s %s", cmd_proc);
        }
        if (strncmp(line, "Tgid", 4) == 0) {
            sscanf(line, "%*s %d", &pid_proc);
        }
        if (strncmp(line, "Uid", 3) == 0) {
            sscanf(line, "%*s %d %*d %*d %*d", &uid_proc);
        }
    }

    if (uid == uid_proc) {
        printf("%d %s\n", pid_proc, cmd_proc);
    }

    if (fclose(fp)) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
    
    free(line);
    
    return;
}

int main(int argc, char *argv[])
{
    DIR *dirp;
    struct dirent *dp;
    uid_t uid;
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s username\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uid = uid_from_name(argv[1]);
    if (uid == -1) {
        fprintf(stderr, "No user found with that name.\n");
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
        
        display_process_if_user(pid, uid);
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
