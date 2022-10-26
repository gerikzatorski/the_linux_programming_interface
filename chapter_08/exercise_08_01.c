/* exercise_08_01.c */

/*********************************************************************
When we execute the following code, we find that it displays the same
number twice, even though the two users have different IDs in the
password file. Why is this?

printf("%ld %ld\n", (long) (getpwnam("avr")->pw_uid),
                    (long) (getpwnam("tsr")->pw_uid));

Note: Using the provided code led to unintended results - the numbers displayed were different. I assume the intention was the demonstrate the non reentrant nature of getpwnam().
*********************************************************************/

#include <pwd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /* Numbers displayed are actually different */
    
    printf("%ld %ld\n", (long) (getpwnam("root")->pw_name),
                        (long) (getpwnam("man")->pw_name));

    /* Added this which prints "root root" */
    
    printf("%s %s\n", (getpwnam("root")->pw_name),
                      (getpwnam("man")->pw_name));

    exit(EXIT_SUCCESS);
}
