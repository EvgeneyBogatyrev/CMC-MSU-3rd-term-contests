#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

enum
{
    STR_LEN = 8,
    BASE = 10,
    PROC_NUM = 3
};

int
main(void)
{
    int id = 0;

    if (!fork()) {
        id = 1;
    } else if (!fork()) {
        id = 2;
    } else if (!fork()) {
        id = 3;
    }

    if (id != 0) {
        unsigned char buf[STR_LEN];

        read(0, buf, sizeof(buf));

        int num = 0;
        for (int i = 0; i < STR_LEN - 1; ++i) {
            num *= BASE;
            num += (buf[i] - '0');
        }

        num = num * num;
        printf("%d %d\n", id, num);
        fflush(stdout);
        _exit(0);
    }

    for (int i = 0; i < PROC_NUM; ++i) {
        wait(NULL);
    }

    return 0;
}
