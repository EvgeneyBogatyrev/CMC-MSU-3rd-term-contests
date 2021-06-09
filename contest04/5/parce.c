#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char **argv)
{
    int fd = open(argv[1], O_CREAT | O_RDWR, 0777);

    if (fd > 0) {

        int N;
        scanf("%d", &N);
        for (int i = 0; i < N; ++i) {

            unsigned short int n;
            scanf("%hu", &n);

            uint8_t out = n >> 8;
            write(fd, &out, 1);

            out = n % 256;
            write(fd, &out, 1);
        }
        close(fd);
    }
    return 0;
}
