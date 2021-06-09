#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int
main(void)
{
    uint32_t n, s, w;
    scanf("%" PRIu32 "%" PRIu32 "%" PRIu32, &n, &s, &w);
    uint32_t num = 0;
    uint32_t end = 1 << n;

    while (num < end) {
        printf("|%*o|", w, num);
        printf("%*" PRIu32 "|", w, num);
        
        uint32_t sign = (num & (1 << (n - 1))) >> (n - 1);
        uint32_t copy = num & (~(1 << (n - 1)));

        int32_t result = (int32_t) copy;
        if (sign) {
            result *= -1;
        }

        printf("%*" PRId32 "|\n", w, result);

        num += s;
    }

    return 0;
}
