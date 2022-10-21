/* exercise_04_01.c */

/*********************************************************************
The tee command reads its standard input until end-of-file, writing a
copy of the input to standard output and to the file named in its
command-line argument. (We show an example of the use of this command
when we discuss FIFOs in Section 44.7.) Implement tee using I/O system
calls. By default, tee overwrites any existing file with the given
name. Implement the –a command-line option (tee –a file), which causes
tee to append text to the end of a file if it already exists. (Refer
to Appendix B for a description of the getopt() function, which can be
used to parse command-line options.)
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/* Allow "cc -D" to override definition */

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int opt, a_flag = 0, output_fd, output_flags;
    mode_t output_perms;
    ssize_t num_read;
    char buf[BUF_SIZE];
    
    while ((opt = getopt (argc, argv, ":a")) != -1) {
        switch (opt) {
            case 'a':
                a_flag = 1;
                break;
            case '?':
                fprintf(stderr, "Unkown option: %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    }

    if (argc != optind + 1) {
        fprintf(stderr, "Usage: %s [-a] file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (a_flag)
        output_flags = O_CREAT | O_WRONLY | O_APPEND;
    else
        output_flags = O_CREAT | O_WRONLY | O_TRUNC;
    output_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                   S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    output_fd = open(argv[optind], output_flags, output_perms);
    if (output_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(output_fd, buf, num_read) != num_read) {
            perror("write (to file)");
            break;
        }
        if (write(STDOUT_FILENO, buf, num_read) != num_read) {
            perror("write (to stdout)");
            break;
        }
    }

    if (num_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    if (close(output_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
