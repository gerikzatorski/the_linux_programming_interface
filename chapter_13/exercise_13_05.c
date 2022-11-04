/* exercise_13_05.c */

/*********************************************************************
The command tail [ –n num ] file prints the last num lines (ten by
default) of the named file. Implement this command using I/O system
calls (lseek(), read(), write(), and so on). Keep in mind the
buffering issues described in this chapter, in order to make the
implementation efficient.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define DEFAULT_NUM_LINES 10
#define MAX_LINE 1028

int print_last_x_lines(int fd, int x)
{
    off_t curr;
    off_t offsets[x + 1];
    int i = 0;
    char c[1];
    ssize_t num_bytes;
    char buf[MAX_LINE];
    int line_len;

    curr = lseek(fd, -1, SEEK_END);
    
    while (curr >= 0 && i < x + 1) {
        num_bytes = read(fd, c, 1);
        if (num_bytes == -1) {
            return -1;
        }

        if (c[0] == '\n') {
            offsets[i++] = curr + 1;
        }

        /* Seek two steps back cause we read one step forward */
        
        curr = lseek(fd, -2, SEEK_CUR);
    }

    if (curr == -1) { /* Handle case where it oversteps SEEK_SET */
        offsets[i] = curr + 1;
    }
    else { /* Otherwise undo increment of i before while() eval */
        i--; 
    }

    while (i > 0) {
        curr = lseek(fd, offsets[i], SEEK_SET);
        line_len = offsets[i-1] - offsets[i];

        num_bytes = read(fd, buf, line_len);
        if (num_bytes == -1) {
            return -1;
        }
        buf[line_len] = '\0';

        printf("%s", buf);
        i--;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    int num_lines = DEFAULT_NUM_LINES;
    int opt;
    int input_fd;
    ssize_t num_bytes;
    off_t offset;
    
    while ((opt = getopt(argc, argv, ":n:")) != -1) {
        switch (opt) {
        case 'n':
            num_lines = atoi(optarg);
            if (num_lines == 0) {
                fprintf(stderr, "Usage: %s [-n num] file\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        case '?':
            fprintf(stderr, "Unkown option: -%c\n", optopt);
            exit(EXIT_FAILURE);
        case ':':
            fprintf(stderr, "Missing arg for %c\n", optopt);
            exit(EXIT_FAILURE);
        }
    }

    if (argc != optind + 1) {
        fprintf(stderr, "Usage: %s [-n num] file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_fd = open(argv[optind], O_RDONLY);
    if (input_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    print_last_x_lines(input_fd, num_lines);

    if (close(input_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_FAILURE);
}
