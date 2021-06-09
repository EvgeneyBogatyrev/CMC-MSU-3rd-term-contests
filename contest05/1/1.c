#include <stdio.h>
#include <time.h>

enum
{
    INIT_YEAR = 1900,
    THURSDAY = 4,
    DIV = 3
};

int
main(void)
{
    int year;
    if (scanf("%d", &year) != 1) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }
    
    int set_year = year - INIT_YEAR;

    struct tm time;
    time.tm_sec = 0;
    time.tm_min = 0;
    time.tm_hour = 0;
    time.tm_mday = 1;
    time.tm_mon = 0;
    time.tm_year = set_year;
    time.tm_isdst = -1;
       
    mktime(&time);
        
    int cur_month = 0;
    int thursday_count = 0;
    while (time.tm_year == set_year) {
        if (cur_month != time.tm_mon) {
            cur_month = time.tm_mon;
            thursday_count = 0;
        }

        if (time.tm_wday == THURSDAY) {
            thursday_count++;
            if (time.tm_mday % DIV != 0 && (thursday_count == 2 || thursday_count == 4)) {
                printf("%d %d\n", time.tm_mon + 1, time.tm_mday);
            }
        }

        time.tm_mday++;
        mktime(&time);
    }

    return 0;
}
