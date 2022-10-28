/* exercise_09_03.c */

/*********************************************************************
Implement initgroups() using setgroups() and library functions for
retrieving information from the password and group files (Section
8.4). Remember that a process must be privileged in order to be able
to call setgroups().
 *********************************************************************/

#include <pwd.h>
#include <errno.h>
#include <limits.h>
#include <grp.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GROUPS NGROUPS_MAX + 1
#define MAX_NAME 64

int initgroupscopy(const char *user, gid_t group)
{
    struct passwd *pwd;
    struct group *grp;
    int num_supplemental = 0;
    gid_t pw_gid;
    int curr;
    int i;
    
    errno = 0;
    pwd = getpwnam(user);
    if (pwd == NULL) {
        if (errno == 0) {
            fprintf(stderr, "User not found.\n");
            return -1;
        }
        else {
            perror("getpwnam");
            return -1;
        }
    }

    pw_gid = pwd->pw_gid;
    
    while ((grp = getgrent()) != NULL)
        for (i = 0; grp->gr_mem[i] != NULL; i++)
            if (strcmp(grp->gr_mem[i], user) == 0)
                num_supplemental++;
    endgrent();

    gid_t grouplist[num_supplemental + 2];

    /* Gather supplemental group ids of which user is a member */

    curr = 0;
    while ((grp = getgrent()) != NULL)
        for (i = 0; grp->gr_mem[i] != NULL; i++)
            if (strcmp(grp->gr_mem[i], user) == 0) {
                grouplist[curr++] = grp->gr_gid;
            }
    endgrent();

    /* Also add non supplemental id and group argument */

    grouplist[curr++] = pw_gid;
    grouplist[curr] = group;
    
    if (setgroups(num_supplemental + 2, grouplist) == -1) {
        perror("setgroups");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    gid_t grouplist[MAX_GROUPS];
    int num_groups, i;
    gid_t fake_gid = 3000;
    uid_t real_uid;
    char real_name[MAX_NAME];
    struct passwd *pwd;

    real_uid = getuid();
    pwd = getpwuid(real_uid);
    strcpy(real_name, pwd->pw_name);
    
    num_groups = getgroups(MAX_GROUPS, grouplist);
    printf("\nBefore call, num_groups = %d\n", num_groups);
    for (i = 0; i < num_groups; i++) {
        printf("%d ", grouplist[i]);
    }
    printf("\n");
    
    if (initgroupscopy(real_name, fake_gid) == -1) {
        return -1;
    }

    num_groups = getgroups(MAX_GROUPS, grouplist);
    printf("After call, num_groups = %d\n", num_groups);
    for (i = 0; i < num_groups; i++) {
        printf("%d ", grouplist[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}
