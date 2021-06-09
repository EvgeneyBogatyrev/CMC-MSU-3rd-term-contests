#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


enum 
{ 
    ARG_CNT = 6
};

int
run(int i, int p[], char *s, char *file)
{
    if (!fork()) {
        if (i == 1) {
            int fd = open(file, O_RDONLY, 0);
            dup2(fd, 0);
            dup2(p[1], 1);

            close(fd);
        } else if (i == 2) {
            dup2(p[1], 1);
        } else if (i == 3) {
            int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            dup2(fd, 1);
            dup2(p[0], 0);

            close(fd);
        }
        close(p[0]);
        close(p[1]);
        printf("%d\n", i);
        fflush(stdout);
        execlp(s, s, NULL);
        _exit(1);
    }
    close(p[0]);
    close(p[1]);
    int status;
    wait(&status);
    return !status;
}

int
main(int argc, char **argv) 
{
    if (argc < ARG_CNT) {
        return 0;
    }

    int p[2];
    pipe(p);

    if (!fork()) {    
        int res = run(1, p, argv[1], argv[4]);
        if (!WEXITSTATUS(res)) {
            printf("OK\n");
            fflush(stdout);
            run(2, p, argv[2], NULL);
        } 
        res = run(3, p, argv[3], argv[5]);
        close(p[0]);
        close(p[1]);
        _exit(res);
    }
    close(p[0]);
    close(p[1]);
    wait(NULL);

    return 0;
}
