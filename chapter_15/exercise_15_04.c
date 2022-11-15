/* exercise_15_04.c */

/*********************************************************************
The access() system call checks permissions using the process’s real
user and group IDs. Write a corresponding function that performs the
checks according to the process’s effective user and group IDs.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

int access_effective(const char *pathname, int mode)
{
    struct stat sb;
    uid_t euid;
    gid_t egid;

    if (stat(pathname, &sb) == -1) { 
        return -1;
    }

    euid = geteuid();
    egid = getegid();

    if (mode & R_OK) { /* Can the file be read? */
        if (sb.st_uid == euid) {
            if (!(sb.st_mode & S_IRUSR))
                return -1;
        }
        else if (sb.st_gid == egid) {
            if (!(sb.st_mode & S_IRGRP))
                return -1;
        }
        else {
            if (!(sb.st_mode & S_IROTH)) {
                return -1;
            }
        }
    }
    
    if (mode & W_OK) { /* Can the file be written? */
        if (sb.st_uid == euid) {
            if (!(sb.st_mode & S_IWUSR))
                return -1;
        }
        else if (sb.st_gid == egid) {
            if (!(sb.st_mode & S_IWGRP))
                return -1;
        }
        else {
            if (!(sb.st_mode & S_IWOTH)) {
                return -1;
            }
        }
    }

    if (mode & X_OK) { /* Can the file be executed? */
        if (sb.st_uid == euid) {
            if (!(sb.st_mode & S_IXUSR))
                return -1;
        }
        else if (sb.st_gid == egid) {
            if (!(sb.st_mode & S_IXGRP))
                return -1;
        }
        else {
            if (!(sb.st_mode & S_IXOTH)) {
                return -1;
            }
        }

    }

    return 0;
}

int main(int argc, char *argv[])
{
    int i = 0;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Access Permissions: ");
    for (i = 1; i < argc; i++) {
        if (access_effective(argv[i], R_OK) == 0) {
            printf("r");
        }
        if (access_effective(argv[i], W_OK) == 0) {
            printf("w");
        }
        if (access_effective(argv[i], X_OK) == 0) {
            printf("x");
        }
    }
    printf("\n");
    
    exit(EXIT_SUCCESS);
}
