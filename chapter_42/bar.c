/* bar.c */

#include <stdio.h>
#include "foo.h"

void say_things()
{
    printf("Bar says hello.\n");
    say_hello();
}
