#include <stdio.h>


const char str[] = "rwxrwxrwx";

int
main(void)
{
    printf("%lu\n", sizeof(str));
    return 0;
}
