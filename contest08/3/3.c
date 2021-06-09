#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

enum
{
    PROC_NUM = 3,
    STR_LEN = 8
};

void
proc_read(int id)
{
    int number = 0;
    unsigned char buffer[STR_LEN];
    if (read(0, buffer, sizeof(buffer)) != STR_LEN) {
        _exit(1);
    }
    sscanf((const char *) buffer, "%d\n", &number);
    number *= number;
    printf("%d %d\n", id, number);
    fflush(stdout);
}

int
main(void)
{
    for (int i = 0; i < PROC_NUM; ++i) {
        int pid;
        if (!(pid = fork())) {
            proc_read(i + 1);
            _exit(0);
        } else if (pid < 0) {
            return 1;
        }
    }

    for (int i = 0; i < PROC_NUM; ++i) {
        wait(NULL);
    }

    return 0;
}
