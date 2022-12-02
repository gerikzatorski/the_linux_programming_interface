/* exercise_19_01.c */

/*********************************************************************
Write a program that logs all file creations, deletions, and renames
under the directory named in its command-line argument. The program
should monitor events in all of the subdirectories under the specified
directory. To obtain a list of all of these subdirectories, you will
need to make use of nftw() (Section 18.9). When a new subdirectory is
added under the tree or a directory is deleted, the set of monitored
subdirectories should be updated accordingly.
 *********************************************************************/

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static int inotify_fd;

int dir_watch(const char *pathname, const struct stat *sbuf,
                int type, struct FTW *ftwb)
{
    int wd;
    
    if (type == FTW_D) {
        printf("Found Directory named %s\n", pathname);
        wd = inotify_add_watch(inotify_fd, pathname,
                               IN_CREATE | IN_DELETE | IN_MOVE);
        if (wd == -1) {
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

static void log_event(struct inotify_event *i)
{
    /* TODO: log particular file or directory */
    
    if (i->mask & IN_CREATE)
        printf("Created %s\n", i->name);
    if (i->mask & IN_DELETE)
        printf("Deleted %s\n", i->name);

    /* Events form an ordered queue (IN_MOVE_FROM before IN_MOVE_TO) */
    
    if (i->mask & IN_MOVED_FROM)
        printf("Renamed %s ", i->name);
    if (i->mask & IN_MOVED_TO)
        printf("to %s\n", i->name);

}

int main(int argc, char *argv[])
{
    int flags;
    char buf[BUF_LEN];
    ssize_t num_read;
    char *p;
    struct inotify_event * event;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory path>", argv[0]);
        exit(EXIT_FAILURE);
    }

    inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }
    
    flags = 0;
    if (nftw(argv[1], dir_watch, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        num_read = read(inotify_fd, buf, BUF_LEN);
        if (num_read ==0) {
            /* TODO: fatal */
            exit(EXIT_FAILURE);
        }

        if (num_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        for (p = buf; p < buf + num_read; ) {
            event = (struct inotify_event *) p;
            log_event(event);
            p += sizeof(struct inotify_event) + event->len;
        }
    }
    
    exit(EXIT_SUCCESS);
}
