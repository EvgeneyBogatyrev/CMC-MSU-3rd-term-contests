#include <stdio.h>

int mystrspccmp(const char *str1, const char *str2);

int
main(void)
{
    const char *str1 = "Hallo";
    const char *str2 = "Hel";
    int result = mystrspccmp(str1, str2);

    printf("%d\n", result);

    return 0;
}
