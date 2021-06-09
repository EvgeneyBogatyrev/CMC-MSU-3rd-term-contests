#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

enum
{
    ARG_NUM = 2,
    BITS = 8
};

int signal_got = 0;
unsigned char data_buffer = 0;
int filled = 0;

void
handler(int sig)
{
    signal_got = sig;
}

void
wait_signal(sigset_t *old_mask)
{
    while (!signal_got) {
        sigsuspend(old_mask);
    }
    if (signal_got == SIGUSR1) {
        filled++;
    } else if (signal_got == SIGUSR2) {
        data_buffer |= (1 << filled);
        filled++;
    } else if (signal_got == SIGIO) {
        _exit(0);
    }

    if (filled == BITS) {
        filled = 0;
        putchar(data_buffer);
        fflush(stdout);
        data_buffer = 0;
    }

    signal_got = 0;
}

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = 0;
    sigaction(SIGUSR1, &sig, NULL); 
    sigaction(SIGUSR2, &sig, NULL);
    sigaction(SIGALRM, &sig, NULL);
    sigaction(SIGIO, &sig, NULL);

    sigset_t mask1, mask2;
    sigemptyset(&mask1);
    sigaddset(&mask1, SIGUSR1);
    sigaddset(&mask1, SIGUSR2);
    sigaddset(&mask1, SIGALRM);
    sigaddset(&mask1, SIGIO);
    sigprocmask(SIG_BLOCK, &mask1, &mask2);
    
    int pfd[2];
    pipe(pfd);
    pid_t pids[2];

    pids[0] = fork();
    if (pids[0] < 0) {
        perror(argv[0]);
        return 1;
    } else if (!pids[0]) {
        
        close(pfd[1]);
        int opponent_pid;
        read(pfd[0], &opponent_pid, sizeof(opponent_pid));
        close(pfd[0]);

        while (1) {
            wait_signal(&mask2);
            kill(opponent_pid, SIGALRM);
        }
        _exit(1);
    }

    pids[1] = fork();
    if (pids[1] < 0) {
        perror(argv[0]);
        return 1;
    } else if (!pids[1]) {

        close(pfd[0]);
        int pid = getpid();
        write(pfd[1], &pid, sizeof(pid));
        close(pfd[1]);

        int fd = open(argv[1], O_RDONLY);

        if (fd < 0) {
            perror(argv[1]);
            _exit(1);
        }

        uint8_t buffer;
        while (read(fd, &buffer, sizeof(buffer)) > 0) {
            for (int i = 0; i < BITS; ++i) {
                if (buffer & 1) {
                    kill(pids[0], SIGUSR2);
                } else {
                    kill(pids[0], SIGUSR1);
                }
                buffer >>= 1;
                wait_signal(&mask2);
            }            
        }
        close(fd);

        kill(pids[0], SIGIO);
        _exit(0);
    }
   
    close(pfd[0]);
    close(pfd[1]);

    while (wait(NULL) > 0) {}
    return 0;
}
