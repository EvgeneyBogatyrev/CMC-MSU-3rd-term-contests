#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

char *relativize_path(const char *, const char *);

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        return 1;
    }

    char *buf = relativize_path(argv[1], argv[2]);
    printf("%s\n", buf);
    free(buf);

    return 0;
}
