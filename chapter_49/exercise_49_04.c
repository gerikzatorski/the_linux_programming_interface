/* exercise_49_04.c */

/*********************************************************************
Write a program that uses the MAP_FIXED technique described in Section
49.10 to create a nonlinear mapping similar to that shown in Figure
49-5.
*********************************************************************/

#include <sys/mman.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *auto_addr, *addr0, *addr1, *addr2;
    off_t off0, off1, off2;
    int fd;
    long page_size;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    page_size = sysconf(_SC_PAGE_SIZE);

    fd = open(argv[1], O_RDWR | O_CREAT,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
        errExit("open");

    if (ftruncate(fd, page_size * 3) == -1)
        errExit("ftruncate");

    /* Create anonymous mapping */

    auto_addr = mmap(NULL, page_size * 3, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (auto_addr == MAP_FAILED)
        errExit("mmap");

    /* Create nonlinear memory mappings */
    
    addr0 = auto_addr + 0 * page_size;
    addr1 = auto_addr + 1 * page_size;
    addr2 = auto_addr + 2 * page_size;

    off0 = 2 * page_size;
    off1 = 1 * page_size;
    off2 = 0 * page_size;
    
    addr0 = mmap(addr0, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, off0);
    if (addr0 == MAP_FAILED)
        errExit("mmap");
    addr1 = mmap(addr1, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, off1);
    if (addr1 == MAP_FAILED)
        errExit("mmap");
    addr2 = mmap(addr2, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, off2);
    if (addr2 == MAP_FAILED)
        errExit("mmap");

    /* Copy "abc" to memory  and ensure file synchronicity */
    
    memcpy(addr0, "a", 1);
    memcpy(addr1, "b", 1);
    memcpy(addr2, "c", 1);

    if (msync(addr0, page_size, MS_SYNC) == -1)
        errExit("msync");
    if (msync(addr1, page_size, MS_SYNC) == -1)
        errExit("msync");
    if (msync(addr2, page_size, MS_SYNC) == -1)
        errExit("msync");
    
    /* Cleanup */
    
    if (munmap(auto_addr, page_size) == -1)
        errExit("munmap");
    if (munmap(addr0, page_size) == -1)
        errExit("munmap");
    if (munmap(addr1, page_size) == -1)
        errExit("munmap");
    if (munmap(addr2, page_size) == -1)
        errExit("munmap");

    if (close(fd) == -1)
        errExit("close");
    
    exit(EXIT_SUCCESS);
}
