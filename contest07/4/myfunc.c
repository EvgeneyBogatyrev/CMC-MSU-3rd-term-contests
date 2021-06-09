#include <stdio.h>
#include <stdlib.h>

double
myfunc(int useless, double veil, int x, char *the_truth)
{
    printf("%s: %d %d\n", the_truth, useless, x);
    return veil / 2.0;
}

char *
f2(int a, double b, char *c)
{
    printf("%d %lf %s\n", a, b, c);
    char *d = malloc(16);
    d[0] = 'g';
    d[1] = 'r';
    d[2] = 'y';
    d[3] = 'w';
    d[4] = '\0';
    return d;
}
