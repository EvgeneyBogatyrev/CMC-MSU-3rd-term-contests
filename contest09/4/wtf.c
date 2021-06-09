#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    INIT_SIZE = 16,
    MODE = 0700
};

const char *filename = "prog.py";

int
main(int argc, char *argv[])
{
    int size = INIT_SIZE;
    char *str = calloc(size, 1);
    strcpy(str, "print(");

    for (int i = 1; i < argc; ++i) {
        if (strlen(str) + strlen(argv[i]) + 1 >= size) {
            size += size;
            str = realloc(str, size);
        }
        strcpy(str + strlen(str), argv[i]);
        if (i != argc - 1) {
            strcpy(str + strlen(str), "*");
        } else {
            strcpy(str + strlen(str), ")");
        }
    }

    int fd = open(filename, O_CREAT | O_WRONLY | O_EXCL, MODE);
    
    if (fd == -1) {
        return 1;
    }

    int written = 0;
    char *p = str;
    
    while ((written = write(fd, p, strlen(p))) > 0) {
        p += written;
    }
    
    free(str);

    close(fd);
    fd = open(filename, O_RDONLY);
    unlink(filename);

    execlp("/usr/bin/python3", "python3", filename, NULL);
    return 1;
}
