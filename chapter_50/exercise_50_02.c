/* exercise_50_02.c */

/*********************************************************************
Write a program to verify the operation of the madvise() MADV_DONTNEED
operation for a writable MAP_PRIVATE mapping.
*********************************************************************/

#include <sys/mman.h>

#include "tlpi_hdr.h"

#define SAMPLE_TEXT "This will be discarded"

int main(int argc, char *argv[])
{
    char *addr;

    addr = mmap(NULL, 1, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    memcpy(addr, SAMPLE_TEXT, strlen(SAMPLE_TEXT));

    /* Verify that mapped pages are discarded */
    
    printf("Before advising: %s\n", addr);

    if (madvise(addr, 1, MADV_DONTNEED) == -1)
        errExit("madvise");

    printf("After advising: %s\n", addr);

    /* Cleanup */
    
    if (munmap(addr, 1) == -1)
        errExit("munmap");

    exit(EXIT_SUCCESS);
}
