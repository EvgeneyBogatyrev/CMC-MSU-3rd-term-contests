#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

int signal_got = 0;

void
handler(int sig)
{
    if (sig == SIGUSR1) {
        signal_got = 1;
    }
}

void
wait_signal(sigset_t *old_mask)
{
    while (!signal_got) {
        sigsuspend(old_mask);
    }
    signal_got = 0;
}

void
work(int id, sigset_t *old_mask, pid_t opponent_pid, int max_number, int out)
{
    int number = 1;
    while (1)
    {
        scanf("%d", &number);
        if (number >= max_number) {
            printf("%d\n", max_number);
            fflush(stdout);
            kill(opponent_pid, SIGUSR1);
            break;
        }
        dprintf(out, "%d %d\n", id, number);
        fflush(stdout);
        number++;
        printf("%d\n", number);
        fflush(stdout);

        kill(opponent_pid, SIGUSR1);
        wait_signal(old_mask);
    }

}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    int max_number;
    sscanf(argv[1], "%d", &max_number);

    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = 0;
    sigaction(SIGUSR1, &sig, NULL);    

    sigset_t mask, old_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, &old_mask);

    int out = dup(1);

    int pfd[2];
    pipe(pfd);

    pid_t pids[2];

    if ((pids[0] = fork()) < 0) {
        perror(argv[0]);
        return 1;
    } else if (pids[0] == 0) {
        dup2(pfd[0], 0);
        dup2(pfd[1], 1);
        close(pfd[0]);
        close(pfd[1]);
        
        pid_t opponent_pid;

        wait_signal(&old_mask);
        scanf("%u", &opponent_pid);
        kill(opponent_pid, SIGUSR1);
        wait_signal(&old_mask);

        work(1, &old_mask, opponent_pid, max_number, out);
        _exit(0);
    }

    if ((pids[1] = fork()) < 0) {
        kill(pids[0], SIGKILL);
        perror(argv[0]);
        return 1;
    } else if (pids[1] == 0) {
        dup2(pfd[0], 0);
        dup2(pfd[1], 1);
        close(pfd[0]);
        close(pfd[1]);

        printf("%u\n", getpid());
        fflush(stdout);
        kill(pids[0], SIGUSR1);
        wait_signal(&old_mask);

        printf("1\n");
        fflush(stdout);
        kill(pids[0], SIGUSR1);
        wait_signal(&old_mask);

        work(2, &old_mask, pids[0], max_number, out);
        _exit(0);
    }

    close(pfd[0]);
    close(pfd[1]);

    wait(NULL);
    wait(NULL);

    printf("Done\n");
    fflush(stdout);
    return 0;
}

