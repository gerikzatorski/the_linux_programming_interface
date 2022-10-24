/* exercise_06_02.c */

/*********************************************************************
Write a program to see what happens if we try to longjmp() into a
function that has already returned.
*********************************************************************/

#include <setjmp.h>
#include <stdlib.h>

static jmp_buf env;

static void x(void)
{
    int var = 4;
    setjmp(env);
    printf("var (should be 4) = %d\n", var);
}

static void y(void)
{
    longjmp(env, 1);
}

int main(int argc, char *argv[])
{
    x();
    y();
    exit(EXIT_SUCCESS);
}
