#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

unsigned long long hash_function(unsigned long long);

enum 
{
    ARG_NUM = 4,
    H_INDEX = 2,
    COUNT_INDEX = 3,
    MODE = 0660
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    const char *filename = (const char *) argv[1];
    unsigned long long h0;
    uint64_t count;

    if (sscanf(argv[H_INDEX], "%llx", &h0) != 1) {
        return 1;   
    }
    if (sscanf(argv[COUNT_INDEX], "%" PRIu64, &count) != 1) {
        return 1;
    }

    int fd = open(filename, O_CREAT | O_WRONLY, MODE);

    if (fd > 0) {
        off_t end = lseek(fd, 0, SEEK_END);
        if (end == -1) {
            return 1;
        }

        for (uint64_t i = 0; i < count; ++i) {
            if (write(fd, &h0, sizeof(h0)) != sizeof(h0)) {
                return 1;
            }
        }
    
        for (uint64_t i = 0; i < count; ++i) {
            if (lseek(fd, end + (count - i - 1) * sizeof(h0), SEEK_SET) == -1 ||
                    write(fd, &h0, sizeof(h0)) != sizeof(h0)) {
                return 1;
            }
            h0 = hash_function(h0);
        }
        close(fd);
    }
    return 0;
}
