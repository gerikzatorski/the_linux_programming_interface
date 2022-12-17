/* exercise_27_02.c */

/*********************************************************************
Use execve() to implement execlp(). You will need to use the stdarg(3)
API to handle the variable-length argument list supplied to execlp().
You will also need to use functions in the malloc package to allocate
space for the argument and environment vectors. Finally, note that an
easy way of checking whether a file exists in a particular directory
and is executable is simply to try execing the file.
*********************************************************************/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define PATH_DELIM ":"
#define MAX_DIRS 100

extern char **environ;

int execlpcopy(const char * filename , const char * arg , ...
/* , (char *) NULL */)
{
    va_list ap;
    char *s, *tok;
    char **pathdirs, **ep;
    int i, j;
    int ndirs = 0, nargs = 1, nenvs = 0; /* basename for nargs */
    char pathname[PATH_MAX + 1];
    
    /* Parse directories in PATH */
    
    pathdirs = malloc(MAX_DIRS * sizeof(char));

    i = 0;
    tok = strtok(getenv("PATH"), PATH_DELIM);
    while (tok != NULL) {
        pathdirs[i] = malloc((strlen(tok) + 1) * sizeof(char));
        strcpy(pathdirs[i], tok);
        
        if (pathdirs[i] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        
        tok = strtok(NULL, PATH_DELIM);
        ndirs++;
        i++;
    }

    /* Allocate memory for argument array */

    va_start(ap, arg);
    while (1) {
        s = va_arg(ap, char *);
        if (s == NULL)
            break;
        nargs++;
    }
    va_end(ap);

    char *arg_array[nargs + 1]; /* Append NULL */

    /* Copy basename into argument vector */

    arg_array[0] = malloc((strlen(arg) + 1) * sizeof(char));
    if (arg_array[0] == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(arg_array[0], arg);

    /* Copy variadic arguments next */
    
    i = 1;
    va_start(ap, arg);
    while (1) {
        s = va_arg(ap, char *);
        if (s == NULL)
            break;
        arg_array[i] = malloc((strlen(s) + 1) * sizeof(char));
        if (arg_array[i] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(arg_array[i], s);
        i++;
    }
    va_end(ap);
    arg_array[nargs] = NULL; /* Terminate list with NULL */

    /* Allocate memeory for environment array */
    
    for (ep = environ; *ep != NULL; ep++) {
        nenvs++;
    }

    char *env_array[nenvs];

    /* Copy environs elements into vector */

    i = 0;
    for (ep = environ; *ep != NULL; ep++) {
        env_array[i] = malloc((strlen(*ep) + 1) * sizeof(char));
        strcpy(env_array[i], *ep);
        i++;
    }
    env_array[nenvs] = NULL; /* Terminate list with NULL */
    
    /* Using execve */

    for (i = 0; i < ndirs; i++) {
        strcpy(pathname, pathdirs[i]);
        strcat(pathname, "/");
        strcat(pathname, arg_array[0]);
        if (execve(pathname, arg_array, env_array) == -1) {
            /* Ignore possibly incorrect pathnames */
        }
    }

    /* All combinations failed if we get here */
    
    return -1;
}

int main(int argc, char *argv[])
{
    execlpcopy("echo", "echo", "hello", "world", (char *) NULL);
    exit(EXIT_SUCCESS);
}
