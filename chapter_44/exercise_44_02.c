/* exercise_44_02.c */

/*********************************************************************
Implement popen() and pclose(). Although these functions are
simplified by not requiring the signal handling employed in the
implementation of system() (Section 27.7), you will need to be careful
to correctly bind the pipe ends to file streams in each process, and
to ensure that all unused descriptors referring to the pipe ends are
closed. Since children created by multiple calls to popen() may be
running at one time, you will need to maintain a data structure that
associates the file stream pointers allocated by popen() with the
corresponding child process IDs. (If using an array for this purpose,
the value returned by the fileno() function, which obtains the file
descriptor corresponding to a file stream, can be used to index the
array.) Obtaining the correct process ID from this structure will
allow pclose() to select the child upon which to wait. This structure
will also assist with the SUSv3 requirement that any still-open file
streams created by earlier calls to popen() must be closed in the new
child process.
*********************************************************************/

#include "tlpi_hdr.h"

#define SHELL ("/bin/sh")
#define PFD_MAX 128

static int pid_map[PFD_MAX];

FILE *popencopy(const char *command, const char *mode)
{
    pid_t child_pid;
    int pfd[2];
    int *parent_fd, *child_fd;

    /* Validate and process mode argument */
    
    if (strcmp(mode, "r") == 0) {
        parent_fd = &pfd[0];
        child_fd = &pfd[1];
    }
    else if (strcmp(mode, "w") == 0) {
        parent_fd = &pfd[1];
        child_fd = &pfd[0];
    }
    else {
        errno = EINVAL;
        return NULL;
    }

    if (pipe(pfd) == -1)
        return NULL;

    switch (child_pid = fork()) {
    case -1:
        return NULL;

    case 0:
        if (close(*parent_fd) == -1)
            return NULL;

        /* Bind pipe ends to relevant streams */
        
        if (strcmp(mode, "r") == 0) {
            if (*child_fd != STDOUT_FILENO)
                if (dup2(*child_fd, STDOUT_FILENO) == -1)
                    return NULL;
        }
        else if (strcmp(mode, "w") == 0) {
            if (*child_fd != STDIN_FILENO)
                if (dup2(*child_fd, STDIN_FILENO) == -1)
                    return NULL;
        }

        if (close(*child_fd) == -1) /* Close superfluous fd */
            return NULL;

        execlp(SHELL, SHELL, "-c", command, (char *) NULL);
        return NULL;

    default:
        if (close(*child_fd) == -1)
            return NULL;
        
        if (*parent_fd >= PFD_MAX) {
            errno = ENOMEM;
            return NULL;
        }
        
        pid_map[*parent_fd] = child_pid;

        return fdopen(*parent_fd, mode);
    }
}

int pclosecopy(FILE *stream)
{
    int fd;
    pid_t pid;
    int status;
    
    fd = fileno(stream);
    if (fd == -1)
        return -1;

    pid = pid_map[fd];
    if (pid == 0) {
        errno = EINVAL;
        return -1;
    }

    if (close(fd) == -1)
        return -1;
    
    /* Wait for and yield the termination status of the child shell */

    if (waitpid(pid, &status, 0) == -1)
        return -1; /* errno set to ECHILD */

    return WEXITSTATUS(status);
}

static int read_demo()
{
    FILE *fp;
    char buf[128];
    
    printf("\nRead demo:\n");

    fp = popencopy("ls", "r");

    while (fgets(buf, sizeof(buf), fp))
        printf("%s", buf);

    pclosecopy(fp);

    return 0;
}

static int write_demo()
{
    FILE *fp;
    int x;

    printf("\nWrite demo:\n");

    fp = popencopy("cat", "w");

    /* Disable buffering since fp doesn't refer to a terminal */
    
    setbuf(fp, NULL);

    for(x = 0; x < 4; x++)
        fprintf(fp, "x = %d\n", x);

    pclosecopy(fp);

    return 0;
}

int main(int argc, char *argv[])
{
    read_demo();
    write_demo();

    exit(EXIT_SUCCESS);
}
