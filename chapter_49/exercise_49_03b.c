/* exercise_49_03b.c */

/*********************************************************************
Write programs to verify that the SIGBUS and SIGSEGV signals are
delivered in the circumstances described in Section 49.4.3.

This program verifies SIGSEGV.
*********************************************************************/

#include <sys/mman.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *addr;
    int fd;
    long page_size;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    page_size = sysconf(_SC_PAGE_SIZE);

    /* Ensure file is small enough */
    
    fd = open(arv[1], O_RDWR | O_CREAT,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
        errExit("open");

    if (ftruncate(fd, page_size) == -1)
        errExit("ftruncate");

    /* Create memory mapping larger than file */

    addr = mmap(NULL, page_size * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        errExit("mmap");

    /* Generate SIGBUS by attempting to access memory beyond mapped memory */

    memcpy(&addr[(int) (page_size * 3)], "big fail", 8);

    /* Cleanup */

    if (munmap(addr, page_size) == -1)
        errExit("munmap");

    if (close(fd) == -1)
        errExit("close");
    
    exit(EXIT_SUCCESS);
}
