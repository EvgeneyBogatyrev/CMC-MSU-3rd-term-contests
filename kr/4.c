#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

enum
{
    LEN = 10,
    YEAR = 4,
    MONTH = 7,
    DAY = 10
};

void
print_date(struct tm time)
{
    printf("%.4d-", time.tm_year + 1900);
    printf("%.2d-", time.tm_mon + 1);
    printf("%.2d\n", time.tm_mday);
}

int
main(void)
{
    char* date = calloc(LEN + 1, 1);
    int years;

    for (int i = 0; i < LEN; ++i) {
        date[i] = getchar();
    }
    getchar();

    scanf("%d", &years);

    int year = 0;
    for (int i = 0; i < YEAR; ++i) {
        year *= 10;
        year += date[i] - '0';
    }

    int month = 0;
    for (int i = YEAR + 1; i < MONTH; ++i) {
        month *= 10;
        month += date[i] - '0';
    }

    int day = 0;
    for (int i = MONTH + 1; i < DAY; ++i) {
        day *= 10;
        day += date[i] - '0';
    }

    uint64_t max_dif = years * 365 * 24 * 60 * 60;

    struct tm time;
    time.tm_sec = 0;
    time.tm_min = 0;
    time.tm_hour = 0;
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = day;
    time.tm_isdst = -1;

    time_t start_time = timegm(&time);
    time_t cur_time = start_time;

    while (1) {
        uint64_t diff = (uint64_t) difftime(cur_time, start_time);

        if (diff >= max_dif) {
            break;
        }

        time.tm_mon += 6;
        cur_time = timegm(&time);

        struct tm copy = time;

        if (copy.tm_wday == 6) {
            copy.tm_mday += 2;
        } else if (copy.tm_wday == 0) {
            copy.tm_mday += 1;
        }

        timegm(&copy);
        print_date(copy);
    }
    free(date);
    return 0;
}
