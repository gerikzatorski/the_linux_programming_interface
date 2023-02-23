/* exercise_38_02.c */

/*********************************************************************
Write a set-user-ID-root program similar to the sudo(8) program. This
program should take command-line options and arguments as follows:

  $ ./douser [-u user ] program-file arg1 arg2 ...

The douser program executes program-file, with the given arguments, as
though it was run by user. (If the –u option is omitted, then user
should default to root.) Before executing program-file, douser should
request the password for user, authenticate it against the standard
password file (see Listing 8-2, on page 164), and then set all of the
process user and group IDs to the correct values for that user.
*********************************************************************/

#if ! defined(__sun)
#define _BSD_SOURCE     /* Get getpass() declaration from <unistd.h> */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE   /* Get crypt() declaration from <unistd.h> */
#endif
#endif
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include <grp.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"

#define DEFAULT_USER "root"

static int authenticate_user(char *username, uid_t *uid, gid_t *gid)
{
    char *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;
    Boolean auth_okay;

    pwd = getpwnam(username);
    if (pwd == NULL)
        fatal("couldn't get password record");

    *uid = pwd->pw_uid;
    *gid = pwd->pw_gid;
  
    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");

    if (spwd != NULL) /* If there is a shadow password record */
        pwd->pw_passwd = spwd->sp_pwdp; /* Use the shadow password */

    password = getpass("Password: ");

    /* Encrypt password and erase cleartext version immediately */

    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0'; )
        *p++ = '\0';

    if (encrypted == NULL)
        errExit("crypt");

    auth_okay = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!auth_okay) {
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

static int get_user_groups(const char* name, gid_t *grouplist)
{
    struct passwd *pwd;
    struct group *grp;
    int i, n = 0;
    
    setgrent();
    while ((grp = getgrent()) != NULL)
        for (i = 0; grp->gr_mem[i] != NULL; i++)
            if (strcmp(grp->gr_mem[i], name) == 0)
                grouplist[n++] = grp->gr_gid;
    endgrent();

    pwd = getpwnam(name);
    if (pwd == NULL)
        fatal("couldn't get password record");

    grouplist[n++] = pwd->pw_gid;
    
    return n;
}

int main(int argc, char *argv[])
{
    int opt, num_groups;
    char *username;
    long lnmax;
    uid_t uid;
    gid_t gid;
    gid_t grouplist[NGROUPS_MAX + 1];

    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1) /* If limit is indeterminate */
        lnmax = 256; /* make a guess */
    
    username = malloc(lnmax);
    if (username == NULL)
        errExit("malloc");
    strcpy(username, DEFAULT_USER);
    
    while ((opt = getopt(argc, argv, ":u:")) != -1) {
        switch(opt) {
        case 'u':
            strcpy(username, optarg);
            break;
        case '?':
            fprintf(stderr, "Unkown option: -%c\n", optopt);
            exit(EXIT_FAILURE);
        case ':':
            fprintf(stderr, "Missing arg for %c\n", optopt);
            exit(EXIT_FAILURE);
        }
    }

    if (!argv[optind])
        usageErr("%s [-u user ] program-file arg1 arg2 ...", argv[0]);

    /* Authenticate and retrieve uid/gid for later system calls */

    authenticate_user(username, &uid, &gid);

    /* Change process credentials uids changed last
       to maintain privilege */

    num_groups = get_user_groups(username, grouplist);
    
    if (setgroups(num_groups, grouplist) == -1)
        errExit("setgroups");

    if (setregid(gid, gid) == -1)
        errExit("setregid");

    if (setreuid(uid, uid) == -1)
        errExit("setreuid");

    /* Execute program-file */
    
    execvp(argv[optind], &argv[optind]);
    errExit("execvp");
}
