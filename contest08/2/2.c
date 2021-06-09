#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int
main(void)
{
    if (!fork()) {
        if (!fork()) {
            printf("3 ");
            fflush(stdout);
            _exit(0);
        }
        wait(NULL);
        printf("2 ");
        fflush(stdout);
        _exit(0);
    }
    wait(NULL);
    printf("1\n");
    return 0;
}
