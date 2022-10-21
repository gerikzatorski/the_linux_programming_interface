/* exercise_03_01.c */

/*********************************************************************
When using the Linux-specific reboot() system call to reboot the
system, the second argument, magic2, must be specified as one of a set
of magic numbers (e.g., LINUX_REBOOT_MAGIC2). What is the significance
of these numbers? (Converting them to hexadecimal provides a clue.)
*********************************************************************/

#include <stdio.h>
#include <linux/reboot.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /* DOB of Linux Torvald */
    
    printf("LINUX_REBOOT_MAGIC2 = 0x%x\n", LINUX_REBOOT_MAGIC2);

     /* DOB of his children */
    
    printf("LINUX_REBOOT_MAGIC2A = 0x%x\n", LINUX_REBOOT_MAGIC2A);
    printf("LINUX_REBOOT_MAGIC2B = 0x%x\n", LINUX_REBOOT_MAGIC2B);
    printf("LINUX_REBOOT_MAGIC2C = 0x%x\n", LINUX_REBOOT_MAGIC2C);
    
    exit(EXIT_SUCCESS);
}
