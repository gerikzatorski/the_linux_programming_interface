/* exercise_16_01.c */

/*********************************************************************
Write a program that can be used to create or modify a user EA for a
file (i.e., a simple version of setfattr(1)). The filename and the EA
name and value should be supplied as command-line arguments to the
program.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/xattr.h>
#include <limits.h>

#define EANAME_MAX 255
#define EAVALUE_MAX 1023 /* TODO: actual limit is 64 kB */

int main(int argc, char *argv[])
{
    int opt, nflag = 0, vflag = 0;
    char pathname[PATH_MAX + 1];
    char eaname[EANAME_MAX + 1];
    char eavalue[EAVALUE_MAX + 1];
    
    while ((opt = getopt(argc, argv, ":n:v:")) != -1) {
        switch (opt) {
        case 'n':
            nflag = 1;
            strcpy(eaname, optarg);
            break;
        case 'v':
            vflag = 1;
            strcpy(eavalue, optarg);
            break;
        case '?':
            fprintf(stderr, "Unkown option: -%c\n", optopt);
            exit(EXIT_FAILURE);
        case ':':
            fprintf(stderr, "Missing arg for %c\n", optopt);
            exit(EXIT_FAILURE);
        }
    }

    if (!nflag || !vflag || optind >= argc) {
        fprintf(stderr, "Usage: %s -n user.name -v value files\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    while (optind < argc) {

        strcpy(pathname, argv[optind++]);
        printf("%s\n", pathname);
        
        /* Will create a new EA by default if name doesn't exist */

        if (setxattr(pathname, eaname, eavalue, strlen(eavalue), 0) == -1) {
            perror("setxattr");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
