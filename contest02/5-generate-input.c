#include <stdio.h>
#include <inttypes.h>

int
main(void)
{
    uint64_t amount = 1250000;
    for (uint64_t i = 0; i < amount; ++i) {
        printf("%" PRIu64 "", i % 10);
    }
    printf("\n");
    return 0;
}
