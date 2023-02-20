/* exercise_49_01.c */

/*********************************************************************
Write a program, analogous to cp(1), that uses mmap() and memcpy()
calls (instead of read() or write()) to copy a source file to a
destination file. (Use fstat() to obtain the size of the input file,
which can then be used to size the required memory mappings, and use
ftruncate() to set the size of the output file.)
*********************************************************************/

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *source_addr, *dest_addr;
    int source_fd, dest_fd;
    struct stat sb;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s source destination\n", argv[0]);

    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1)
        errExit("open");

    dest_fd = open(argv[2], O_RDWR | O_CREAT,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (dest_fd == -1)
        errExit("open");

    /* Set output file size using source file size */

    if (fstat(source_fd, &sb) == -1)
        errExit("fstat");

    if (sb.st_size == 0) /* Handle empy source file */
        exit(EXIT_SUCCESS);

    if (ftruncate(dest_fd, sb.st_size) == -1)
        errExit("ftruncate");

    /* Create memory mappings */
    
    source_addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, source_fd, 0);
    if (source_addr == MAP_FAILED)
        errExit("mmap");

    dest_addr = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, dest_fd, 0);
    if (dest_addr == MAP_FAILED)
        errExit("mmap");

    /* Copy memory and ensure file synchronicity */
    
    memcpy(dest_addr, source_addr, sb.st_size);

    if (msync(dest_addr, sb.st_size, MS_SYNC) == -1)
        errExit("msync");
    
    /* Cleanup */

    if (munmap(source_addr, sb.st_size) == -1)
        errExit("munmap");
    
    if (munmap(dest_addr, sb.st_size) == -1)
        errExit("munmap");
    
    if (close(source_fd) == -1)
        errExit("close");

    if (close(dest_fd) == -1)
        errExit("close");
    
    exit(EXIT_SUCCESS);
}
