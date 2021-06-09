#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <inttypes.h>

enum
{
    START = 2,
    MAX_COUNT = 4
};

int counter = 0;
int signal_got = 0;

void
handler(int sig)
{
    if (sig == SIGINT) {
        counter++;
        if (counter == MAX_COUNT) {
            _exit(0);
        }
        signal_got = 1;
    } else if (sig == SIGTERM) {
        _exit(0);
    }
}

int
is_prime(uint64_t num)
{
    if (num == 0 || num == 1) {
        return 0;
    }

    for (uint64_t i = START; i * i <= num; ++i) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(void)
{
    uint64_t low, high;
    scanf("%" SCNu64 "%" SCNu64, &low, &high);

    printf("%u\n", getpid());
    fflush(stdout);

    struct sigaction sig;
    sig.sa_handler = handler;
    sig.sa_flags = 0;
    sigaction(SIGINT, &sig, NULL);
    sigaction(SIGTERM, &sig, NULL);

    uint64_t last_prime = 0;

    for (uint64_t num = low; num < high; ++num) {
        if (is_prime(num)) {
            last_prime = num;
        }

        if (signal_got) {
            printf("%" PRId64 "\n", last_prime);
            fflush(stdout);
            signal_got = 0;
        }
    }
    
    printf("-1\n");
    fflush(stdout);

    return 0;
}
