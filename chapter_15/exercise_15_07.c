/* exercise_15_07.c */

/*********************************************************************
Write a simple version of the chattr(1) command, which modifies file
i-node flags. See the chattr(1) man page for details of the chattr
command-line interface. (You don’t need to implement the –R, –V, and
–v options.)
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd, i, j, attr;
    char pathname[PATH_MAX + 1 ];
    
    if (argc < 3) {
        fprintf(stderr, "Usage: %s +-=[acDijAdtsSTu] file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(pathname, argv[argc - 1]);

    /* Open with mode=0 to prevent permission issues */
    
    fd = open(pathname, 0);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (ioctl(fd, FS_IOC_GETFLAGS, &attr) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc - 1; i++) {
        if (argv[i][0] == '+') {
            for (j = 1; j < strlen(argv[i]); j++) {
                switch(argv[i][j]) {
                case 'a':
                    attr |= FS_APPEND_FL;
                    break;
                case 'c':
                    attr |= FS_COMPR_FL;
                    break;
                case 'D':
                    attr |= FS_DIRSYNC_FL;
                    break;
                case 'i':
                    attr |= FS_IMMUTABLE_FL;
                    break;
                case 'j':
                    attr |= FS_JOURNAL_DATA_FL;
                    break;
                case 'A':
                    attr |= FS_NOATIME_FL;
                    break;
                case 'd':
                    attr |= FS_NODUMP_FL;
                    break;
                case 't':
                    attr |= FS_NOTAIL_FL;
                    break;
                case 's':
                    attr |= FS_SECRM_FL;
                    break;
                case 'S':
                    attr |= FS_SYNC_FL;
                    break;
                case 'T':
                    attr |= FS_TOPDIR_FL;
                    break;
                case 'u':
                    attr |= FS_UNRM_FL;
                    break;
                default:
                    fprintf(stderr, "Unrecognized option +%c\n", argv[i][j]);
                    exit(EXIT_FAILURE);
                }
            }
        } else if (argv[i][0] == '-') {
            for (j = 1; j < strlen(argv[i]); j++) {
                switch(argv[i][j]) {
                case 'a':
                    attr = attr & ~FS_APPEND_FL;
                    break;
                case 'c':
                    attr = attr & ~FS_COMPR_FL;
                    break;
                case 'D':
                    attr = attr & ~FS_DIRSYNC_FL;
                    break;
                case 'i':
                    attr = attr & ~FS_IMMUTABLE_FL;
                    break;
                case 'j':
                    attr = attr & ~FS_JOURNAL_DATA_FL;
                    break;
                case 'A':
                    attr = attr & ~FS_NOATIME_FL;
                    break;
                case 'd':
                    attr = attr & ~FS_NODUMP_FL;
                    break;
                case 't':
                    attr = attr & ~FS_NOTAIL_FL;
                    break;
                case 's':
                    attr = attr & ~FS_SECRM_FL;
                    break;
                case 'S':
                    attr = attr & ~FS_SYNC_FL;
                    break;
                case 'T':
                    attr = attr & ~FS_TOPDIR_FL;
                    break;
                case 'u':
                    attr = attr & ~FS_UNRM_FL;
                    break;
                default:
                    fprintf(stderr, "Unrecognized option +%c\n", argv[i][j]);
                    exit(EXIT_FAILURE);
                }
            }

        } else if (argv[i][0] == '=') {
            attr = 0;
            for (j = 1; j < strlen(argv[i]); j++) {
                switch(argv[i][j]) {
                case 'a':
                    attr |= FS_APPEND_FL;
                    break;
                case 'c':
                    attr |= FS_COMPR_FL;
                    break;
                case 'D':
                    attr |= FS_DIRSYNC_FL;
                    break;
                case 'i':
                    attr |= FS_IMMUTABLE_FL;
                    break;
                case 'j':
                    attr |= FS_JOURNAL_DATA_FL;
                    break;
                case 'A':
                    attr |= FS_NOATIME_FL;
                    break;
                case 'd':
                    attr |= FS_NODUMP_FL;
                    break;
                case 't':
                    attr |= FS_NOTAIL_FL;
                    break;
                case 's':
                    attr |= FS_SECRM_FL;
                    break;
                case 'S':
                    attr |= FS_SYNC_FL;
                    break;
                case 'T':
                    attr |= FS_TOPDIR_FL;
                    break;
                case 'u':
                    attr |= FS_UNRM_FL;
                    break;
                default:
                    fprintf(stderr, "Unrecognized option +%c\n", argv[i][j]);
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            fprintf(stderr, "Usage: %s +-=[acDijAdtsSTu] file\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (ioctl(fd, FS_IOC_SETFLAGS, &attr) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
