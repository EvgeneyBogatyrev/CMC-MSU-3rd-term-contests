#include <stdio.h>
#include <math.h>

enum 
{ 
    ARG_START = 2,
    TOTAL = 100,
    FACTOR = 10000
};

int
main(int argc, char **argv)
{
    double cost;
    if (sscanf(argv[1], "%lf", &cost) != 1) {
        fprintf(stderr, "Wrong number of argumets\n");
        return 1;
    }

    for (int i = ARG_START; i < argc; ++i) {
        double dif;
        sscanf(argv[i], "%lf", &dif);
        cost = cost * (1 + dif / TOTAL);
        cost = round(cost * FACTOR) / FACTOR;
    }

    printf("%.4lf\n", cost);
    return 0;
}
