/* exercise_50_01.c */

/*********************************************************************
Verify the effect of the RLIMIT_MEMLOCK resource limit by writing a
program that sets a value for this limit and then attempts to lock
more memory than the limit.
*********************************************************************/

#include <sys/mman.h>
#include <sys/resource.h>
#include "tlpi_hdr.h"

#define LEN 1

int main(int argc, char *argv[])
{
    char *addr;
    struct rlimit rlim;

    /* Change rlimit value */
    
    if (getrlimit(RLIMIT_MEMLOCK, &rlim) == -1)
        errExit("getrlimit");
    
    rlim.rlim_cur = 0;
    
    if (setrlimit(RLIMIT_MEMLOCK, &rlim) == -1)
        errExit("getrlimit");

    /* Attempt to create locked mapping */
    
    addr = mmap(NULL, LEN, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, -1, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    /* Cleanup */
    
    if (munmap(addr, LEN) == -1)
        errExit("munmap");

    exit(EXIT_SUCCESS);
}
