/* exercise_08_02.c */

/*********************************************************************
Implement getpwnam() using setpwent(), getpwent(), and endpwent().
*********************************************************************/

#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static struct passwd *getpwnamcopy(const char *name)
{
    static struct passwd *pwd;  /* Static pointer like getpwnam() */

    while ((pwd = getpwent()) != NULL) {
        if (strcmp(name, pwd->pw_name) == 0) {
            endpwent();
            return pwd;
        }
    }

    endpwent();
    
    return NULL; /* User not found */
}

int main(int argc, char *argv[])
{
    struct passwd *pwd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s username\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pwd = getpwnamcopy(argv[1]);
    if (pwd == NULL) {
        fprintf(stderr, "Passwd entry for %s not found.\n", argv[1]);
        exit(EXIT_FAILURE);
    }
        
    printf("pw_name = %s\n", pwd->pw_name);
    printf("pw_shell = %s\n", pwd->pw_shell);
    
    exit(EXIT_SUCCESS);
}
