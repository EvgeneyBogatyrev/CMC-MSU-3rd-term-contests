#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

int
main(int argc, char **argv)
{
    const char *filename = (const char *) argv[1];

    int fd = open(filename, O_CREAT | O_WRONLY, 0777);

    int32_t arr[] = {14, 1, 0, 11, 0, 0};
    int len = 6;

    for (int i = 0; i < len; ++i) {
        int32_t data = arr[i];
        write(fd, &data, 4);
    }


    close(fd);

    return 0;
}
