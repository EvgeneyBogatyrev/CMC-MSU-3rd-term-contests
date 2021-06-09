#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

enum 
{ 
    BIT_NUM = 8,
    BYTE_NUM = 2
};

int
main(int argc, char **argv)
{
    if (argc <= 1) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd > 0) {

        uint16_t number;
        uint8_t buffer[BYTE_NUM];
    
        int min = -1;

        while (1) {
            number = 0;

            int len = read(fd, &buffer, sizeof(buffer));
            if (len <= 0) {
                break;
            }
            number |= buffer[0];
            number <<= BIT_NUM;
            number |= buffer[1];
            
            if (!(number & 1) && (min == -1 || min > number)) {
                min = number;
            }

        }
        close(fd);

        if (min != -1) {
            printf("%u\n", min);
        }
    }

    return 0;
}
