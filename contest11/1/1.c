#include <stdio.h>
#include <unistd.h>
#include <signal.h>

enum
{
    SIG_NUM = 5
};

int counter = 0;

void
handler(int signal)
{
    if (signal == SIGHUP) {
        printf("%d\n", counter);
        fflush(stdout);
        counter++;
    }
}

int
main(void)
{
    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = 0;
    sigaction(SIGHUP, &sig, NULL);

    sigset_t new_mask, old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGHUP);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    printf("%d\n", getpid());
    fflush(stdout);
    while (counter < SIG_NUM) {
        sigsuspend(&old_mask);
    }
    return 0;
}
