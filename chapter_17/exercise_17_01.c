/* exercise_17_01.c */

/*********************************************************************
Write a program that displays the permissions from the ACL entry that
corresponds to a particular user or group. The program should take two
command-line arguments. The first argument is either of the letters u
or g, indicating whether the second argument identifies a user or
group. (The functions defined in Listing 8-1, on page 159, can be used
to allow the second command-line argument to be specified numerically
or as a name.) If the ACL entry that corresponds to the given user or
group falls into the group class, then the program should additionally
display the permissions that would apply after the ACL entry has been
modified by the ACL mask entry.
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/acl.h>
#include <acl/libacl.h>
#include <grp.h>
#include <pwd.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

int acl_filtered_permissions(acl_permset_t group, acl_permset_t specific, acl_permset_t mask)
{
    int perm_val;
    int r, w, x;

    /* Copy permissions from obj */
    
    if (group != NULL) {
        perm_val = acl_get_perm(group, ACL_READ);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_READ");
            return -1;
        }
        r = perm_val;
        perm_val = acl_get_perm(group, ACL_WRITE);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_WRITE");
            return -1;
        }
        w = perm_val;
        perm_val = acl_get_perm(group, ACL_EXECUTE);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_EXECUTE");
            return -1;
        }
        x = perm_val;
    }

    /* Overwrite permissions from specific entry */
    
    if (specific != NULL) {
        perm_val = acl_get_perm(specific, ACL_READ);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_READ");
            return -1;
        }
        r = perm_val;
        perm_val = acl_get_perm(specific, ACL_WRITE);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_WRITE");
            return -1;
        }
        w = perm_val;
        perm_val = acl_get_perm(specific, ACL_EXECUTE);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_EXECUTE");
            return -1;
        }
        x = perm_val;
    }

    /* Apply mask to permissions */
    
    if (mask != NULL) {
        perm_val = acl_get_perm(mask, ACL_READ);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_READ");
            return -1;
        }
        if (perm_val == 1) {
            r = 0;
        }
        perm_val = acl_get_perm(mask, ACL_WRITE);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_WRITE");
            return -1;
        }
        if (perm_val == 1) {
            w = 0;
        }
        perm_val = acl_get_perm(mask, ACL_EXECUTE);
        if (perm_val == -1) {
            perror("acl_get_perm - ACL_EXECUTE");
            return -1;
        }
        if (perm_val == 1) {
            x = 0;
        }
    }

    printf("Permissions: %c%c%c\n", r ? 'r' : '-', w ? 'w' : '-', x ? 'x' : '-');
    return 0;
}

int acl_uid_permissions(const char *filepath, uid_t uid)
{
    acl_t acl;
    acl_type_t type;
    acl_entry_t entry;
    acl_tag_t tag_type;
    int entry_id;

    uid_t *uidp;
    acl_permset_t user_obj_permset, user_permset, mask_permset;

    user_obj_permset = NULL;
    user_permset = NULL;
    mask_permset = NULL;
    
    type = ACL_TYPE_ACCESS;
    acl = acl_get_file(filepath, ACL_TYPE_ACCESS);
    if (acl == NULL) {
        return -1;
    }

    for (entry_id = ACL_FIRST_ENTRY;; entry_id = ACL_NEXT_ENTRY) {
        if (acl_get_entry(acl, entry_id, &entry) != 1) {
            break; /* Exit on error or no more entries */
        }

        if (acl_get_tag_type(entry, &tag_type) == -1) {
            return -1;
        }

        if (tag_type == ACL_USER) {
            uidp = acl_get_qualifier(entry);
            if (uidp == NULL) {
                return -1;
            }

            /* Only record user permset if it matches query */
            
            if (*uidp == uid) {
                if (acl_get_permset(entry, &user_permset) == -1) {
                    return -1;
                }
            }
            
            if (acl_free(uidp) == -1) {
                return -1;
            }
        }
        else if (tag_type == ACL_USER_OBJ) {
            if (acl_get_permset(entry, &user_obj_permset) == -1) {
                return -1;
            }
        } else if (tag_type == ACL_MASK) {
            if (acl_get_permset(entry, &mask_permset) == -1) {
                return -1;
            }
        }
    }

    acl_filtered_permissions(user_obj_permset, user_permset, mask_permset);
    
    if (acl_free(acl) == -1) {
        return -1;
    }

    return 0;
}

int acl_gid_permissions(const char *filepath, gid_t gid)
{
    acl_t acl;
    acl_type_t type;
    acl_entry_t entry;
    acl_tag_t tag_type;
    int entry_id;

    gid_t *gidp;
    acl_permset_t user_obj_permset, user_permset, mask_permset;

    user_obj_permset = NULL;
    user_permset = NULL;
    mask_permset = NULL;
    
    type = ACL_TYPE_ACCESS;
    acl = acl_get_file(filepath, ACL_TYPE_ACCESS);
    if (acl == NULL) {
        return -1;
    }

    for (entry_id = ACL_FIRST_ENTRY;; entry_id = ACL_NEXT_ENTRY) {
        if (acl_get_entry(acl, entry_id, &entry) != 1) {
            break; /* Exit on error or no more entries */
        }

        if (acl_get_tag_type(entry, &tag_type) == -1) {
            return -1;
        }

        if (tag_type == ACL_USER) {
            gidp = acl_get_qualifier(entry);
            if (gidp == NULL) {
                return -1;
            }

            /* Only record user permset if it matches query */
            
            if (*gidp == gid) {
                if (acl_get_permset(entry, &user_permset) == -1) {
                    return -1;
                }
            }
            
            if (acl_free(gidp) == -1) {
                return -1;
            }
        }
        else if (tag_type == ACL_USER_OBJ) {
            if (acl_get_permset(entry, &user_obj_permset) == -1) {
                return -1;
            }
        } else if (tag_type == ACL_MASK) {
            if (acl_get_permset(entry, &mask_permset) == -1) {
                return -1;
            }
        }
    }

    acl_filtered_permissions(user_obj_permset, user_permset, mask_permset);
    
    if (acl_free(acl) == -1) {
        return -1;
    }

    return 0;
}

/* From Listing 8-1 */

uid_t user_id_from_name(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;
    if (name == NULL || *name == '\0')
        return -1;

    u = strtol(name, &endptr, 10);
    if (*endptr == '\0')
        return u;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;

    return pwd->pw_uid;
}

/* From Listing 8-1 */

gid_t group_id_from_name(const char *name)
{
    struct group *grp;
    gid_t g;
    char *endptr;

    if (name == NULL || *name == '\0')
        return -1;

    g = strtol(name, &endptr, 10);
    if (*endptr == '\0')
        return g;

    grp = getgrnam(name);
    if (grp == NULL)
        return -1;

    return grp->gr_gid;
}

int isnumber(const char *s) {
    int i;

    for (i = 0; i < strlen(s); i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    
    return 1; /* All characters are digits */
}

int main(int argc, char *argv[])
{
    char filepath[PATH_MAX + 1];
    uid_t uid;
    gid_t gid;
    
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <u|g> <user|group> file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "u") == 0) {
        if (isnumber(argv[2])) {
            uid = atol(argv[2]);
        }
        else {
            uid = user_id_from_name(argv[2]);
        }
        acl_uid_permissions(argv[3], uid);
    }
    else if (strcmp(argv[1], "g") == 0) {
        if (isnumber(argv[2])) {
            gid = atol(argv[2]);
        }
        else {
            gid = user_id_from_name(argv[2]);
        }
        acl_gid_permissions(argv[3], gid);
    }
    else {
        fprintf(stderr, "Usage: %s <u|g> <user|group> file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
