/* exercise_44_01.c */

/*********************************************************************
Write a program that uses two pipes to enable bidirectional
communication between a parent and child process. The parent process
should loop reading a block of text from standard input and use one of
the pipes to send the text to the child, which converts it to
uppercase and sends it back to the parent via the other pipe. The
parent reads the data coming back from the child and echoes it on
standard output before continuing around the loop once more.
*********************************************************************/

#include "tlpi_hdr.h"

#define BUF_SIZE 10

static int uppercase(char *s)
{
    int i;

    for (i = 0; i < strlen(s); i++)
        if(s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 32;
}

int main(int argc, char *argv[])
{
    int pfd1[2];
    int pfd2[2];
    char buf[BUF_SIZE];
    ssize_t num_read;

    setbuf(stdout, NULL); /* Disable buffering of stdout */

    if (pipe(pfd1) == -1)
        errExit("pipe");

    if (pipe(pfd2) == -1)
        errExit("pipe");

    switch(fork()) {
    case -1:
        errExit("fork");

    case 0: /* Child writes to pfd2 and reads from pfd1 */

        if (close(pfd2[0]) == -1)
            errExit("close");
        if (close(pfd1[1]) == -1)
            errExit("close");
        for(;;) {
            num_read = read(pfd1[0], buf, BUF_SIZE);
            if (num_read == -1)
                errExit("read");
            if (num_read == 0)
                break;

            uppercase(buf);
        
            if (write(pfd2[1], buf, num_read) != num_read)
                fatal("partial/failed write");
        }
        _exit(EXIT_SUCCESS);

    default: /* Parent writes to pfd1 and reads from pfd2 */
        
        if (close(pfd1[0]) == -1)
            errExit("close");
        if (close(pfd2[1]) == -1)
            errExit("close");

        for(;;) {
            printf("Enter text: ");
            if (fgets(buf, BUF_SIZE, stdin) == NULL)
                break;

            /* Newline is captured by fgets */
            
            if (buf[strlen(buf) - 1] == '\n')
                buf[strlen(buf) - 1] = '\0';

            /* Terminate given empty input */
            
            if (strlen(buf) == 0)
                break;
            
            if (write(pfd1[1], buf, BUF_SIZE) != BUF_SIZE)
                fatal("partial/failed write");

            if (read(pfd2[0], buf, BUF_SIZE) == -1)
                errExit("read");

            printf("%s\n", buf);
        }
        
        break;
    }
    
    exit(EXIT_SUCCESS);
}
