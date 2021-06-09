#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

enum { BASE = 10 };

int
main(int argc, char **argv)
{
    int64_t pos_sum = 0;
    int64_t neg_sum = 0;

    for (int i = 1; i < argc; ++i) {
        int num = (int) strtol(argv[i], NULL, BASE);
        if (num < 0) {
            neg_sum += num;
        } else {
            pos_sum += num;
        }
    }

    printf("%" PRId64 "\n%" PRId64 "\n", pos_sum, neg_sum);
    return 0;
}
