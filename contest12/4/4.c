#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    ARG_NUM = 3,
    BUF_SIZE = 128,
    MODE = 0751,
    MOD_NUM = 2
};

const char *script = "#! /usr/bin/env python3\na = int(input())\nprint(((1 + a) * a // 2) % ";

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, MODE);
    if (fd < 0) {
        perror(argv[0]);
        return 1;
    }

    write(fd, script, strlen(script));
    write(fd, argv[MOD_NUM], strlen(argv[MOD_NUM]));
    write(fd, ")\n", 2);

    close(fd);
    return 0;
}
