#include <stdio.h>
#include <stdlib.h>

char *relativize_path(const char *path1, const char *path2);

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        return 1;
    }

    char *buffer = relativize_path(argv[1], argv[2]);
    printf("%s\n", buffer);
    free(buffer);

    return 0;
}
