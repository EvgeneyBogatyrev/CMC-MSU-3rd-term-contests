#include <stdio.h>

int
main(void)
{
    fprintf(stderr, "Something\n");

    int num;
    scanf("%d", &num);
    printf("%d\n", num + 1);

    return 0;
}
