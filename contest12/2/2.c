#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum
{
    ARG_NUM = 3,
    BUF_SIZE = 4096
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    int count, modulo;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &modulo);

    int pfd2to4[2];
    pipe(pfd2to4);

    pid_t p1_pid = fork();
    if (p1_pid < 0) {
        perror(argv[0]);
        return 1;
    } else if (!p1_pid) {
        int pfd1to2[2];
        pipe(pfd1to2);

        pid_t p2_pid = fork();
        if (p2_pid < 0) {
            perror(argv[0]);
            _exit(1);
        } else if (!p2_pid) {
            close(pfd1to2[1]);
            close(pfd2to4[0]);

            dup2(pfd1to2[0], 0);
            dup2(pfd2to4[1], 1);
            
            close(pfd1to2[0]);
            close(pfd2to4[1]);

            execlp(argv[3], argv[3], NULL);
            perror(argv[3]);
            _exit(1);
        }

        close(pfd1to2[0]);

        FILE *fd = fdopen(pfd1to2[1], "w");
        for (int i = 1; i <= count; ++i) {
            long long data = (((long long)(i % modulo)) * i) % modulo;
            fprintf(fd, "%d ", (int) data);
            fflush(stdout);
        }
        fclose(fd);

        close(pfd2to4[0]);
        close(pfd2to4[1]);

        wait(NULL);
        _exit(0);
    }

    pid_t p3_pid = fork();
    if (p3_pid < 0) {
        perror(argv[0]);
        return 1;
    } else if (!p3_pid) {
        pid_t p4_pid = fork();
        if (p4_pid < 0) {
            perror(argv[0]);
            _exit(1);
        } else if (!p4_pid) {
            close(pfd2to4[1]);
            
            char buf[BUF_SIZE];
            off_t sz;
            while ((sz = read(pfd2to4[0], buf, sizeof(buf) - 1)) > 0) {
                for (int i = 0; i < sz; ++i) {
                    if (buf[i] == ' ') {
                        buf[i] = '\n';
                    }
                }
                write(1, buf, sz);
            }
            
            close(pfd2to4[0]);
            _exit(0);
        }

        close(pfd2to4[0]);
        close(pfd2to4[1]);
        wait(NULL);
        _exit(0);
    }

    close(pfd2to4[0]);
    close(pfd2to4[1]);

    wait(NULL);
    wait(NULL);
    printf("0\n");
    return 0;
}
