#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <ctype.h>

enum
{
    BYTE_LEN = 16,
    BASE = 10
};

int
main(void)
{
    unsigned char buffer[BYTE_LEN];

    uint64_t sum = 0;
    uint64_t current_number = 0;
    int sign = 1;

    while (1) {
        int count = read(0, buffer, sizeof(buffer));
        
        if (count <= 0) {
            break;
        }

        for (int i = 0; i < count; ++i) {
            if (isspace(buffer[i])) {
                if (sign > 0) {
                    sum += current_number;
                } else {
                    sum -= current_number;
                }
                current_number = 0;
                sign = 1;
            } else if (buffer[i] == '-') {
                sign = -1;
            } else if (buffer[i] == '+') {
                sign = 1;
            } else if (isdigit(buffer[i])) {
                current_number *= BASE;
                current_number += buffer[i] - '0';
            }
        }
    }

    printf("%" PRId64 "\n", sum);
    return 0;
}
