#include <stdio.h>
#include <time.h>
#include <inttypes.h>

enum
{
    ARG_NUM = 3,
    INIT_YEAR = 1925,
    INIT_MONTH = 10,
    INIT_DAY = 7,
    START_YEAR = 1900,
    SECONDS_IN_DAY = 24 * 60 * 60,
    DAYS_IN_MONTH = 30,
    DAYS_IN_YEAR = 30 * 12
};

int
main(void)
{
    struct tm buffer;
    buffer.tm_year = INIT_YEAR - START_YEAR;
    buffer.tm_mon = INIT_MONTH - 1;
    buffer.tm_mday = INIT_DAY;
    buffer.tm_isdst = -1;
    buffer.tm_sec = 0;
    buffer.tm_min = 0;
    buffer.tm_hour = 0;
   
    time_t start_time = timegm(&buffer);

    int year, month, day;
    while (scanf("%d%d%d", &year, &month, &day) == ARG_NUM) {
        struct tm cur_date;
        cur_date.tm_year = year - START_YEAR;
        cur_date.tm_mon = month - 1;
        cur_date.tm_mday = day;
        cur_date.tm_isdst = -1;
        cur_date.tm_sec = 0;
        cur_date.tm_min = 0;
        cur_date.tm_hour = 0;
    
        time_t cur_time = timegm(&cur_date);

        uint64_t diff = (uint64_t) difftime(cur_time, start_time);
        
        diff /= SECONDS_IN_DAY;

        printf("%" PRIu64 " ", diff / DAYS_IN_YEAR + 1);
        diff %= DAYS_IN_YEAR;
        
        printf("%" PRIu64 " ", diff / DAYS_IN_MONTH + 1);
        diff %= DAYS_IN_MONTH;

        printf("%" PRIu64 "\n", diff + 1);
    }
    return 0;
}
