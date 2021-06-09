#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>

enum
{
    INC = 5,
    DEC = 4,
    SIG1 = SIGUSR1,
    SIG2 = SIGUSR2
};

int counter = 0;

void
handler(int sig)
{
    if (sig == SIG1) {
        counter += INC;
        printf("%d %d\n", sig, counter);
        fflush(stdout);
    } else if (sig == SIG2) {
        counter -= DEC;
        printf("%d %d\n", sig, counter);
        fflush(stdout);
    }
}


int
main(void)
{ 
    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = 0;
    sigaction(SIG1, &sig, NULL);
    sigaction(SIG2, &sig, NULL);

    sigset_t mask, old_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIG1);
    sigaddset(&mask, SIG2);
    sigprocmask(SIG_BLOCK, &mask, &old_mask);

    printf("%u\n", getpid());
    fflush(stdout);

    while (counter >= 0) {
        sigsuspend(&old_mask);
    }

    return 0;
}
