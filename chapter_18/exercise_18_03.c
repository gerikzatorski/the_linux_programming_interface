/* exercise_18_03.c */

/*********************************************************************
Implement realpath().
*********************************************************************/

#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

char *realpathcopy(const char *pathname, char *resolved_path)
{
    char start_path[PATH_MAX + 1];
    int l, r, curr_len;
    char curr_file[NAME_MAX + 1];
    
    /* Caller responsible to free this memory */
    
    if (resolved_path == NULL) {
        resolved_path = malloc(PATH_MAX + 1);
    }

    /* Determine if reading absolute or relative path */
    
    if (pathname[0] == '/') {
        strcpy(start_path, "/");
        l = 0;
        r = 1;
    }
    else {
        getcwd(start_path, PATH_MAX);
        l = -1;
        r = 0;
    }

    chdir(start_path);

    while (r < strlen(pathname)) {
        if (pathname[r] == '/') {
            curr_len = r - l - 1;
            strncpy(curr_file, pathname + l + 1, curr_len);
            curr_file[curr_len] = '\0';
            chdir(curr_file);
            l = r;
        }
        r++;
    }

    /* Last file in pathname remains */

    curr_len = r - l - 1;
    strncpy(curr_file, pathname + l + 1, curr_len);
    curr_file[curr_len] = '\0';
    chdir(curr_file);
    l = r;
    
    getcwd(resolved_path, PATH_MAX);
    return resolved_path;
}

int main(int argc, char *argv[])
{
    char pathname[PATH_MAX + 1];
    char resolved_path[PATH_MAX + 1];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s pathname", argv[0]);
        exit(EXIT_FAILURE);
    }

    strcpy(pathname, argv[1]);
    realpathcopy(pathname, resolved_path);

    printf("resolved_path = %s\n", resolved_path);
    exit(EXIT_SUCCESS);
}
