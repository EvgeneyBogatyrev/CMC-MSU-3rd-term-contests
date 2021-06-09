#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum 
{ 
    ARG_NUM = 2,
    MODE = 0600,
    BUFFER_LEN = 4,
    MASK_ODD = ~0,
    MASK_EVEN = (1 << 4) - 1
};

int
main(int argc, char **argv)
{
    if (argc < ARG_NUM) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    const char *filename = (const char *) argv[1];
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, MODE);

    unsigned number;
    while (scanf("%u", &number) == 1) {
        unsigned char result[BUFFER_LEN];

        result[3] = (number & MASK_ODD);
        result[2] = ((number >> 8) & MASK_EVEN);
        result[1] = ((number >> 12) & MASK_ODD);
        result[0] = ((number >> 20) & MASK_EVEN);
   
        write(fd, result, sizeof(result));
    }

    close(fd);
    return 0;
}
