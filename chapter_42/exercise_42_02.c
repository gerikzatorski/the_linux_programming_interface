/* exercise_42_02.c */

/*********************************************************************
Add a dladdr() call to the program in Listing 42-1 ( dynload.c ) in
order to retrieve information about the address returned by dlsym().
Print out the values of the fields of the returned Dl_info structure,
and verify that they are as expected.
*********************************************************************/

#define _GNU_SOURCE
#include <dlfcn.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    void *lib_handle;
    void (*funcp)(void);
    const char *err;
    Dl_info info;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s lib-path func-name\n", argv[0]);

    lib_handle = dlopen(argv[1], RTLD_LAZY);
    if (lib_handle == NULL)
        fatal("dlopen: %s", dlerror());

    /* This differs from book version. See erratum note
       for page 864 at http://www.man7.org/tlpi/errata/. */

    (void) dlerror();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    funcp = (void (*)(void)) dlsym(lib_handle, argv[2]);
#pragma GCC diagnostic pop

    err = dlerror();
    if (err != NULL)
        fatal("dlsym: %s", err);

    (*funcp)();

    if (dladdr(funcp, &info) == 0)
        fatal("dladdr: %s", dlerror());

    /* Print some Dl_info values */

    printf("pathname of shared lib  = %s\n", info.dli_fname);
    printf("nearest run-time symbol = %s\n", info.dli_sname);
        
    dlclose(lib_handle);

    exit(EXIT_SUCCESS);
}
