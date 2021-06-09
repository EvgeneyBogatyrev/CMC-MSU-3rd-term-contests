#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    ARG_NUM = 6
};

int
main(int argc, char *argv[])
{
    int pfd1to2[2];
    pipe(pfd1to2);
    if (!fork()) {
        dup2(pfd1to2[1], 1);
        close(pfd1to2[1]);
        close(pfd1to2[0]);
        execlp(argv[1], argv[1], NULL);
        perror(argv[1]);
        _exit(1);
    }

    wait(NULL);

    int pfd2to3[2];
    pipe(pfd2to3);

    if (!fork()) {
        close(pfd1to2[1]);
        dup2(pfd1to2[0], 0);
        close(pfd1to2[0]);

        close(pfd2to3[0]);
        dup2(pfd2to3[1], 1);
        close(pfd2to3[1]);

        pid_t pid = fork();
        if (!pid) {
            int ferr = open(argv[5], O_CREAT | O_WRONLY | O_TRUNC, 0600);
            dup2(ferr, 2);
            close(ferr);
            execlp(argv[2], argv[2], NULL);
            perror(argv[2]);
            _exit(1);
        }

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            if (!fork()) {
                execlp(argv[3], argv[3], NULL);
                perror(argv[3]);
                _exit(1);
            }
            wait(NULL);
        }
    }

    close(pfd1to2[0]);
    close(pfd1to2[1]);
    wait(NULL);

    if (!fork()) {
        close(pfd2to3[1]);
        dup2(pfd2to3[0], 0);
        close(pfd2to3[0]);

        execlp(argv[4], argv[4], NULL);
        perror(argv[4]);
        _exit(1);
    }

    close(pfd2to3[0]);
    close(pfd2to3[1]);
    
    while (wait(NULL) > 0) {}

    return 0;
}
