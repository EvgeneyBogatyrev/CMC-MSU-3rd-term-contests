#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <inttypes.h>

int signal_got = 0;

void 
handler(int sig)
{
    signal_got = sig;
}

void
wait_signal(sigset_t *mask)
{
    while (!signal_got) {
        sigsuspend(mask);
    }
}


int
main(int argc, char *argv[])
{
    int pfd[2];
    pipe(pfd);

    pid_t pid;

    if (!(pid = fork())) {
        dup2(pfd[0], 0);
        close(pfd[0]);
        close(pfd[1]);
        execlp(argv[1], argv[1], NULL);
        perror(argv[1]);
        _exit(1);
    }
    
    close(pfd[0]);

    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = 0;
    sigaction(SIGUSR1, &sig, NULL);
    sigaction(SIGUSR2, &sig, NULL);

    sigset_t m1, m2;
    sigemptyset(&m1);
    sigaddset(&m1, SIGUSR1);
    sigaddset(&m1, SIGUSR2);
    sigprocmask(SIG_BLOCK, &m1, &m2);

    int fd = open(argv[2], O_RDONLY);

    int end = 0;
    while (1) {
        wait_signal(&m2);

        if (signal_got && end) {
            kill(pid, SIGUSR1);
            break;
        }

        if (signal_got == SIGUSR1) {
            int32_t sum = 0, num;
            for (int i = 0; i < 10; ++i) {
                off_t sz = read(fd, &num, sizeof(num));
                if (sz == 0) {
                    end = 1;
                    break;
                }
                sum += num;
            }
            write(pfd[1], &sum, sizeof(sum));
        } else if (signal_got == SIGUSR2) {
            uint32_t prod = 0, num;
            for (int i = 0; i < 10; ++i) {
                off_t sz = read(fd, &num, sizeof(num));
                if (sz == 0) {
                    end = 1;
                    break;
                }
                prod *= num;
            }
            dprintf(pfd[1], "%" PRIu32 "\n", prod);
            fflush(stdout);
        }
        signal_got = 0;
    }

    close(fd);
    close(pfd[1]);
    wait(NULL);
    return 0;
}
