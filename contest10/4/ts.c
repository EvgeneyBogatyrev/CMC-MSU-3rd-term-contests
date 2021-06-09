#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

int
close_pipe(int p[2]);
void
killall(int pids[], int end);

int
main(int argc, char **argv)
{
    if (argc == 1) { // no commands to run
        return 0;
    } // else:
    int *pids = malloc((argc - 1) * sizeof(*pids));
    if (pids == NULL) {
        kill(0, SIGSEGV);
        return 1;
    }
    memset(pids, 0, (argc - 1) * sizeof(*pids));
    int p[2];
    memset(p, 0, sizeof(p));
    for (int i = 0; i < argc - 1; ++i) {
        if (pipe(p) < 0) { // pipe error
            killall(pids, i);
            free(pids);
            return 1;
        }
        if ((pids[i] = fork()) < 0) { // fork error
            close_pipe(p);
            killall(pids, i);
            free(pids);
            return 1;
        } else if (pids[i] == 0) { // son
            if (i < argc - 2) { // the last one should print to stdout
                dup2(p[1], 1);
            }
            close_pipe(p);
            free(pids);
            execlp(argv[i + 1], argv[i + 1], NULL);
            _exit(1);
        } // parent:
        dup2(p[0], 0); // we don't need stdin anymore:)
        if (close_pipe(p) < 0) {
            killall(pids, i);
            free(pids);
            return 1;
        }
    }

    free(pids);
    while (wait(NULL) > 0) ; // waiting for children to terminate
    return (errno != ECHILD); // another errno is an error in wait
}

int
close_pipe(int p[2])
{
    int status = 0;
    status += close(p[0]);
    status += close(p[1]);
    return -(status < 0);
}

void
killall(int pids[], int end)
{
    // send SIGKILL
    for (int i = 0; i < end; ++i) {
        kill(pids[i], SIGKILL);
    }
    // wait for termination
    while (wait(NULL) > 0) ;
}

