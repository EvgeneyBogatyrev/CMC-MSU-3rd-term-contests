#include <stdio.h>
#include <ctype.h>

int
main(void)
{
    int c;
    while ((c = getchar()) != EOF) {
        if (!isspace(c)) {
            printf("%c", c + 1);
        } else {
            putchar(c);
        }
    }
    return 0;
}
