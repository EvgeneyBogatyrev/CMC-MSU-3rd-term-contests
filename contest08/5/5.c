#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(void)
{
    int pid;
    if (!(pid = fork())) {
        int number;
        while (scanf("%d", &number) == 1) {
            if (!(pid = fork())) {
                continue;
            } else if (pid < 0) {
                _exit(1);
            } else {
                int status;
                wait(&status);
                if (WEXITSTATUS(status)) {
                    _exit(1);
                }
                printf("%d\n", number);
                fflush(stdout);
                _exit(0);
            }
        }
        _exit(0);
    } else if (pid < 0) {
        printf("-1\n");
        return 0;
    }

    int status;
    wait(&status);
    if (WEXITSTATUS(status)) {
        printf("-1\n");
    }
    return 0;
}   
