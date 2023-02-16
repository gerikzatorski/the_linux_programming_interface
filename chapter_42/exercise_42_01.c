/* exercise_42_01.c */

/*********************************************************************
Write a program to verify that if a library is closed with dlclose(),
it is not unloaded if any of its symbols are used by another library.
*********************************************************************/

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include "bar.h"

#define LIB_FOO_PATH "./libfoo.so"
#define LIB_BAR_PATH "./libbar.so"

#define FOO_SYMBOL "say_hello"

int main(int argc, char *argv[])
{
    void *foo_handle, *bar_handle;
    void (*funcp)(void);
    const char *err;
    
    foo_handle = dlopen(LIB_FOO_PATH, RTLD_LAZY);
    if (foo_handle == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    bar_handle = dlopen(LIB_BAR_PATH, RTLD_LAZY);
    if (bar_handle == NULL) {
        fprintf(stderr, "dlopen: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    
    (void) dlerror(); /* Clear dlerror() */
    *(void **) (&funcp) = dlsym(foo_handle, FOO_SYMBOL);
    err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "dlsym: %s\n", err);
        exit(EXIT_FAILURE);
    }

    /* Try calling function from libfoo */

    if (funcp == NULL)
        printf("%s is NULL\n", FOO_SYMBOL);
    else
        (*funcp)();

    printf("Closing Foo library.\n");
    dlclose(foo_handle);

    (void) dlerror(); /* Clear dlerror() */
    *(void **) (&funcp) = dlsym(foo_handle, FOO_SYMBOL);
    err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "dlsym: %s\n", err);
        exit(EXIT_FAILURE);
    }
    
    /* Try calling function from libfoo */

    if (funcp == NULL)
        printf("%s is NULL\n", FOO_SYMBOL);
    else
        (*funcp)();

    exit(EXIT_SUCCESS);
}
