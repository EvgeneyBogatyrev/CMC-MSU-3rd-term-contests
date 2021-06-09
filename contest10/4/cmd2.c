#include <stdio.h>

int
main(void)
{
    int c, counter = 0;
    while ((c = getchar()) != EOF) {
        counter++;
    }
    printf("%d\n", counter);
    return 0;
}
