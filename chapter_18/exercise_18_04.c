/* exercise_18_04.c */

/*********************************************************************
Modify the program in Listing 18-2 (list_files.c) to use readdir_r()
instead of readdir().
*********************************************************************/

#if defined(__APPLE__)
        /* Darwin requires this header before including <dirent.h> */
#include <sys/types.h>
#endif
#include <dirent.h>
#include "tlpi_hdr.h"

static void             /* List all files in directory 'dirpath' */
listFiles(const char *dirpath)
{
    DIR *dirp;
    Boolean isCurrent;          /* True if 'dirpath' is "." */
    struct dirent *entry;
    struct dirent *result;

    isCurrent = strcmp(dirpath, ".") == 0;

    dirp = opendir(dirpath);
    if (dirp  == NULL) {
        errMsg("opendir failed on '%s'", dirpath);
        return;
    }

    entry = malloc(sizeof(struct dirent));
    if (entry == NULL)
        errExit("malloc");

    /* For each entry in this directory, print directory + filename */

    for (;;) {
        errno = readdir_r(dirp, entry, &result);

        if (result == NULL)
            break;
        
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;           /* Skip . and .. */

        if (!isCurrent)
            printf("%s/", dirpath);
        printf("%s\n", entry->d_name);
    }

    if (errno != 0)
        errExit("readdir_r");

    if (closedir(dirp) == -1)
        errMsg("closedir");
}

int
main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [dir-path...]\n", argv[0]);

    if (argc == 1)              /* No arguments - use current directory */
        listFiles(".");
    else
        for (argv++; *argv; argv++)
            listFiles(*argv);

    exit(EXIT_SUCCESS);
}
