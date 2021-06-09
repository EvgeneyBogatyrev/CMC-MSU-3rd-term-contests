#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    ARG_NUM = 6,
    PROC_NUM = 2,
    MODE = 0777,
    CMD1 = 1,
    CMD2 = 2,
    CMD3 = 3,
    FILE1 = 4,
    FILE2 = 5
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        fprintf(stderr, "To few argumnets\n");
        return 1;
    }

    int pfd[2];
    pipe(pfd);

    pid_t son_pid;

    if (!(son_pid = fork())) {
        dup2(pfd[1], 1);
        close(pfd[1]);
        close(pfd[0]);

        pid_t pid;
        if (!(pid = fork())) {
            int fin = open(argv[FILE1], O_RDONLY | O_CLOEXEC);
            dup2(fin, 0);
            close(fin);

            execlp(argv[CMD1], argv[CMD1], NULL);
            perror(argv[CMD1]);
            _exit(1);
        }

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            if (!fork()) {
                execlp(argv[CMD2], argv[CMD2], NULL);
                perror(argv[CMD2]);
                _exit(1);
            }
            wait(NULL);
        }
        _exit(0);
    } else if (son_pid < 0) {
        perror(argv[0]);
        return 1;
    }

    if (!(son_pid = fork())) {
        int fout = open(argv[FILE2], O_CREAT | O_WRONLY| O_APPEND | O_CLOEXEC, MODE);
        dup2(fout, 1);
        close(fout);
        dup2(pfd[0], 0);
        close(pfd[0]);
        close(pfd[1]);

        execlp(argv[CMD3], argv[CMD3], NULL);
        perror(argv[CMD3]);
        _exit(1);
    } else if (son_pid < 0) {
        perror(argv[0]);
        return 1;
    }

    close(pfd[0]);
    close(pfd[1]);

    for (int i = 0; i < PROC_NUM; ++i) {
        wait(NULL);
    }

    return 0;
}
