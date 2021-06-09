#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

enum 
{
    BIT_NUM = 32
};

int
main(void)
{
    int n, width;
    uint32_t s;
    scanf("%d%" PRIu32 "%d", &n, &s, &width);
    
    uint32_t num = 0;

    uint32_t limit;
    if (n < BIT_NUM) {
        limit = (1u << n) - 1;
    } else {
        limit = ~0;
    }

    while (1) {
        printf("|%*" PRIo32 "|", width, num);
        printf("%*" PRIu32 "|", width, num);
        
        uint32_t bit = num & (1u << (n - 1));
        bit >>= n - 1;

        int32_t result = (int32_t) num;
        result &= ~(1u << (n - 1));
  
        if (bit & 1) {
            printf("%*" PRId32 "|\n", width, -result);
        } else {
            printf("%*" PRIu32 "|\n", width, num);
        }
       
        if (limit - num < s) {
            break;
        }
        num += s;
    }

    return 0;
}
