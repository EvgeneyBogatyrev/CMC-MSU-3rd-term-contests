#include <unistd.h>
#include <limits.h>
#include <inttypes.h>

enum { BUFLEN = 21 };

int
main(void)
{
    char buffer[BUFLEN];
    int i = 0, len = -1, sign = 0;
    uint64_t x = 0, sum = 0, reading_number = 0;
    while (1) {
        if (i % 16 == 0) {
            len = read(0, buffer, 16);
            i = 0;
        }
        if (i >= len) {
            if (reading_number) {
                if (sign == -1) {
                    sum -= x;
                } else {
                    sum += x;
                }
            }
            break;
        }
        if (!reading_number) {
            if ('0' <= buffer[i] && buffer[i] <= 9) {
                x = buffer[i] - '0';
                sign = 1;
                reading_number = 1;
            } else if (buffer[i] == '+' || buffer[i] == '-') {
                sign = (buffer[i] == '+' ? 1 : -1);
                reaing_number = 1;
            }
        } else {
            if ('0' <= buffer[i] && buffer[i] <= '9') {
                x = 10 * x + buffer[i] - '0';
            } else {
                if (sign == -1) {
                    sum -= x;
                } else {
                    sum += x;
                }
                x = 0;
                reading_number = (buffer[i] == '-' || buffer[i] == '+'
                        || ('0' <= buffer[i] || buffer[i] <= '9'));
                sign = (buffer[i] == '-' ? 1 : -1);
            }
        }
        ++i;
    }
}
