#include <stdio.h>
#include <unistd.h>
#include <signal.h>

enum Modes
{
    ADD,
    MULT
};

int mode = ADD;

void
handler(int signal)
{
    if (signal == SIGINT) {
        mode = ADD;
    } else if (signal == SIGQUIT) {
        mode = MULT;
    }
}

int
main(void)
{
    signal(SIGINT, handler);
    signal(SIGQUIT, handler);
    
    printf("%d\n", getpid());
    fflush(stdout);

    int res = 0;
    int cur;
    while (scanf("%d", &cur) == 1) {
        if (mode == ADD) {
            __builtin_sadd_overflow(res, cur, &res); 
        } else if (mode == MULT) {
            __builtin_smul_overflow(res, cur, &res); 
        }
        printf("%d\n", res);
        fflush(stdout);
    }

    return 0;
}
