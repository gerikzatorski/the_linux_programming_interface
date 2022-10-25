/* exercise_06_03.c */

/************************************************************
Implement setenv() and unsetenv() using getenv(), putenv(), and, where
necessary, code that directly modifies environ. Your version of
unsetenv() should check to see whether there are multiple definitions
of an environment variable, and remove them all (which is what the
glibc version of unsetenv() does).
*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern char **environ;

int setenvcopy(const char *name, const char *value, int overwrite)
{
    char *new_str;
    size_t len;

    if (getenv(name) != NULL && overwrite == 0) {
        return 0;
    }

    else {
        len = strlen(name) + strlen(value) + 2;
        new_str = (char *) malloc(len);
        snprintf(new_str, len, "%s=%s", name, value);
        if (putenv(new_str) != 0) {
            free(new_str);
            perror("putenv");
            return -1;
        }
        return 0;
    }
    
}    

int unsetenvcopy(const char *name)
{
    /* putenv() removes variable when argument doesn't contain '=' */

    #ifdef __GNUC__
    putenv((char *) name);
    return 0;
    #else
    #error Only GNU C solution implemented.
    return -1;
    #endif
}

void print_environ()
{
    char **ep;
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    printf("\n");
}

int main(int argc, char *argv[])
{
    print_environ();

    setenvcopy("TEST", "this/is/a/test", 0);
    print_environ();

    setenvcopy("TEST", "this/is/overwritten", 1);
    print_environ();

    unsetenvcopy("TEST");
    print_environ();
    
    exit(EXIT_SUCCESS);
}
