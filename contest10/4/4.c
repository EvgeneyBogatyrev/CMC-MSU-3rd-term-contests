#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <signal.h>

void
kill_children(pid_t *children, int last_one)
{
    for (int i = 0; i < last_one; ++i) {
        kill(children[i], SIGKILL);
        waitpid(children[i], NULL, 0);
    }
}

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        return 0;
    }

    pid_t *children = calloc(argc - 1, sizeof(*children));

    int pfd[2];
    pfd[0] = 0;
    pfd[1] = 1;

    for (int i = 0; i < argc - 1; ++i) {

        int fin = pfd[0];

        if (i != argc - 2 && pipe2(pfd, O_CLOEXEC) < 0) {
            kill_children(children, i);
            free(children);
            return 1;
        }

        if ((children[i] = fork()) < 0) {
            kill_children(children, i);
            free(children);
            return 1;

        } else if (!children[i]) {
            free(children);

            dup2(fin, 0);
            dup2(pfd[1], 1);

            execlp(argv[i + 1], argv[i + 1], NULL);
            perror(argv[i + 1]);
            _exit(1);
        }

        int close_data = 0;
        if (i != 0) {
            close_data += close(fin);
        }

        if (i != argc - 2) {
            close_data += close(pfd[1]);
        }

        if (close_data != 0) {
            kill_children(children, i);
            free(children);
            return 1;
        }

    }

    while (wait(NULL) != -1);

    free(children);
    return 0;
}
