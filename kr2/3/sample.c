/* -*- mode: c; c-basic-offset: 4 -*- */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <unistd.h>
#include <signal.h>

static void handler(int s)
{
    _exit(0);
}

int main(int argc, char *argv[])
{
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);

    int state = 0;
    while (1) {
        if (state == 0) {
            kill(getppid(), SIGUSR1);
            int x;
            read(0, &x, sizeof(x));
            printf("0 %d\n", x); fflush(stdout);
            state = x & 1;
        } else if (state == 1) {
            kill(getppid(), SIGUSR2);
            char buf[1024];
            int z = read(0, buf, sizeof(buf));
            while (z > 0 && isspace(buf[z - 1])) --z;
            buf[z] = 0;
            int x = strtol(buf, NULL, 10);
            printf("1 %d\n", x); fflush(stdout);
            state = x & 1;
        }
    }
}
