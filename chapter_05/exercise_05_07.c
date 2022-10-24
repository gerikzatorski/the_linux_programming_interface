/* exercise_05_07.c */

/*********************************************************************
Implement readv() and writev() using read(), write(), and suitable
functions from the malloc package (Section 7.1.2).
*********************************************************************/

#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

ssize_t readvcopy(int fd, const struct iovec *iov, int iovcnt) {
    unsigned long total_len = 0;
    size_t offset = 0;
    void *buf;
    ssize_t num_read;

    /* Validate input like readv() */
    /* IOV_MAX not declared in limits.h */

    if (iovcnt <= 0) {
        errno = EINVAL;
        return -1;
    }
    
    for (int i = 0; i < iovcnt; i++) {
        total_len += iov[i].iov_len;
    }

    /* Avoid overflow for implicit conversion in read() */

    if (total_len > SSIZE_MAX) {
        errno = EINVAL;
        return -1;
    }

    buf = malloc(total_len);
    if (buf == NULL) {
        perror("malloc");
        return -1;
    }

    /* One read for atmocity */

    num_read = read(fd, buf, total_len);
    if (num_read == -1) {
        free(buf);
        perror("read");
        return -1;
    }

    for (int i = 0; i < iovcnt; i++) {
        memcpy(iov[i].iov_base, buf + offset, iov[i].iov_len);
        offset += iov[i].iov_len;
    }

    free(buf);

    return num_read;
}

ssize_t writevcopy(int fd, const struct iovec *iov, int iovcnt) {
    unsigned long total_len = 0;
    size_t offset = 0;
    void *buf;
    ssize_t num_bytes;

    /* Validate input like readv() */
    /* IOV_MAX not declared in limits.h */
    
    if (iovcnt <= 0) {
        errno = EINVAL;
        return -1;
    }

    for (int i = 0; i < iovcnt; i++) {
        total_len += iov[i].iov_len;
    }

    /* Avoid overflow for implicit conversion in write() */
    
    if (total_len > SSIZE_MAX) {
        errno = EINVAL;
        return -1;
    }

    buf = malloc(total_len);
    if (buf == NULL) {
        perror("malloc");
        return -1;
    }

    for (int i = 0; i < iovcnt; i++) {
        memcpy(buf + offset, iov[i].iov_base, iov[i].iov_len);
        offset += iov[i].iov_len;
    }
    
    /* One write for atmocity */

    num_bytes = write(fd, buf, total_len);
    if (num_bytes == -1) {
        free(buf);
        perror("read");
        return -1;
    }
    
    free(buf);

    return num_bytes;
}

#include <fcntl.h>
#include <assert.h>

#define STR_SIZE 4

int main(int argc, char *argv[]) {
    int fd;
    struct iovec iov_write[2], iov_read[2];
    ssize_t num_bytes, total_len = 0;

    /* Arbitrary values to write and read back */

    int write_int = 64;
    int read_int;
    char write_str[STR_SIZE] = "abc";
    char read_str[STR_SIZE] = "xyz";
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Write data to file */
    
    fd = open(argv[1], O_WRONLY | O_CREAT);
    if (fd == -1) {
        perror("open (for write)");
        exit(EXIT_FAILURE);
    }

    iov_write[0].iov_base = &write_int;
    iov_write[0].iov_len = sizeof(write_int);
    total_len += iov_write[0].iov_len;

    iov_write[1].iov_base = write_str;
    iov_write[1].iov_len = sizeof(write_str);
    total_len += iov_write[1].iov_len;

    num_bytes = writevcopy(fd, iov_write, 2);
    if (num_bytes == -1) {
        perror("writev");
        exit(EXIT_FAILURE);
    }

    if (num_bytes != total_len) {
        fprintf(stderr, "Partial write");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close (for write)");
        exit(EXIT_FAILURE);
    }

    /* Read data back from file */

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open (for read)");
        exit(EXIT_FAILURE);
    }
    
    iov_read[0].iov_base = &read_int;
    iov_read[0].iov_len = sizeof(read_int);

    iov_read[1].iov_base = read_str;
    iov_read[1].iov_len = sizeof(read_str);

    num_bytes = readvcopy(fd, iov_read, 2);
    if (num_bytes == -1) {
        perror("readvcopy");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close (for read)");
        exit(EXIT_FAILURE);
    }
    
    assert(write_int == read_int);
    assert(strcmp(write_str, read_str) == 0);

    exit(EXIT_SUCCESS);
}
