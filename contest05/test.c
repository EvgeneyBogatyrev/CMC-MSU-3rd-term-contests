#include <stdio.h>
#include <string.h>

int
main(void)
{
    printf("%d\n", strncmp("ab", "cd", 0));
    return 0;
}
