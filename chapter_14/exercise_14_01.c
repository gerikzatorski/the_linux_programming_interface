/* exercise_14_01.c */

/*********************************************************************
Write a program that measures the time required to create and then
remove a large number of 1-byte files from a single directory. The
program should create files with names of the form xNNNNNN, where
NNNNNN is replaced by a random six-digit number. The files should be
created in the random order in which their names are generated, and
then deleted in increasing numerical order (i.e., an order that is
different from that in which they were created). The number of files
(NF) and the directory in which they are to be created should be
specifiable on the command line. Measure the times required for
different values of NF (e.g., in the range from 1000 to 20,000) and
for different file systems (e.g., ext2, ext3, and XFS). What patterns
do you observe on each file system as NF increases? How do the various
file systems compare? Do the results change if the files are created
in increasing numerical order (x000001, x000001, x0000002, and so on)
and then deleted in the same order? If so, what do you think the
reason(s) might be? Again, do the results vary across file-system
types?
*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DIR_MODE 0777
#define FILE_MAX 255

void random_number_fill(int numbers[], int count)
{
    int i, r;

    srand(time(NULL));

    for (i = 0; i < count; i++) {
        r = rand() % 1000000;
        numbers[i] = r;
    }
}

void selection_sort(int numbers[], int count)
{
    int i, j, min, tmp;
    for (i = 0; i < count - 1; i++) {
        min = i;
        for (j = i + 1; j < count; j++) {
            if (numbers[j] < numbers[min]) {
                min = j;
            }
        }
        tmp = numbers[i];
        numbers[i] = numbers[min];
        numbers[min] = tmp;
    }
}

int main(int argc, char *argv[])
{
    time_t t_start, t_end;
    char dirpath[PATH_MAX + 1 - FILE_MAX];
    char filepath[PATH_MAX + 1];
    int *numbers;
    int num_files, i, fd;
    ssize_t num_bytes;
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s destination number-of-files\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(dirpath, argv[1]);
    i = strlen(dirpath);
    if (dirpath[i - 1] != '/') {
        dirpath[i++] = '/';
        dirpath[i++] = '\0';
    }

    num_files = atoi(argv[2]);
    if (num_files == 0) {
        fprintf(stderr, "Usage: %s destination number-of-files\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    numbers = malloc(num_files * sizeof(int));
    random_number_fill(numbers, num_files);
    
    t_start = time(NULL);

    if (mkdir(dirpath, DIR_MODE) == -1) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < num_files; i++) {
        sprintf(filepath, "%s%06d", dirpath, numbers[i]);

        fd = open(filepath,
                  O_WRONLY | O_CREAT | O_TRUNC,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if (write(fd, "a", 1) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        if (close(fd) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }

    selection_sort(numbers, num_files);
    
    for (i = 0; i < num_files; i++) {
        sprintf(filepath, "%s%06d", dirpath, numbers[i]);
        remove(filepath);
    }

    remove(dirpath);

    t_end = time(NULL);
    printf("Time elapsed = %f seconds\n", difftime(t_end, t_start));

    free(numbers);
    exit(EXIT_SUCCESS);
}
