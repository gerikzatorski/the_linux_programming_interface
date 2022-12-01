/* exercise_18_07.c */

#define _XOPEN_SOURCE 500
#include <ftw.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

static unsigned int count_reg = 0;    /* regular file */
static unsigned int count_dir = 0;    /* directory */
static unsigned int count_chr = 0;    /* character special file */
static unsigned int count_blk = 0;    /* block special file */
static unsigned int count_lnk = 0;    /* symbolic link */
static unsigned int count_ifo = 0;    /* FIFO special file or a pipe */
static unsigned int count_sock = 0;   /* socket */
static unsigned int count_tot = 0;

static int dir_count(const char *pathname, const struct stat *sbuf,
                     int type, struct FTW *ftwb)
{
    switch (sbuf->st_mode & S_IFMT) {
    case S_IFREG:  count_reg++;  count_tot++; break;
    case S_IFDIR:  count_dir++;  count_tot++; break;
    case S_IFCHR:  count_chr++;  count_tot++; break;
    case S_IFBLK:  count_blk++;  count_tot++; break;
    case S_IFLNK:  count_lnk++;  count_tot++; break;
    case S_IFIFO:  count_ifo++;  count_tot++; break;
    case S_IFSOCK: count_sock++; count_tot++; break;
    default:       break;
    }

    return 0;
}

void print_stats(const char *category, const unsigned int numerator,
                 const unsigned int denominator)
{
    printf("%s: %u (%f %%)\n",
           category, numerator, (float) numerator/denominator * 100);
}

int main(int argc, char *argv[])
{
    int flags;

    if (nftw((argc > 1) ? argv[1] : ".", dir_count, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }

    print_stats("Regular File", count_reg, count_tot);
    print_stats("Directory", count_dir, count_tot);
    print_stats("Character Special File", count_chr, count_tot);
    print_stats("Block Special File", count_blk, count_tot);
    print_stats("Symbolic Link", count_lnk, count_tot);
    print_stats("FIFO Special file or a Pipe", count_ifo, count_tot);
    print_stats("Socket", count_sock, count_tot);
    
    exit(EXIT_SUCCESS);
}
