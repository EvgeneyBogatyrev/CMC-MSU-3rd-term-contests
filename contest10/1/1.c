#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

enum
{
    START_YEAR = 1900
};

int
main(void)
{
    int pfd[2];
    pipe(pfd);

    if (!fork()) {
        if (!fork()) {
            close(pfd[0]);
            
            time_t cur_time = time(NULL);
            struct tm *time = localtime((const time_t *) &cur_time);

            int day = time->tm_mday;
            int month = time->tm_mon + 1;
            int year = time->tm_year + START_YEAR;

            printf("D:%02d\n", day);
            fflush(stdout);

            write(pfd[1], &month, sizeof(month));
            write(pfd[1], &year, sizeof(year));

            _exit(0);
        }
        wait(NULL);
        close(pfd[1]);

        int month;
        read(pfd[0], &month, sizeof(month));
        printf("M:%02d\n", month);
        fflush(stdout);

        _exit(0);
    }
    wait(NULL);
    close(pfd[1]);

    int year;
    read(pfd[0], &year, sizeof(year));
    printf("Y:%04d\n", year);
    fflush(stdout);

    close(pfd[0]);

    return 0;
}
