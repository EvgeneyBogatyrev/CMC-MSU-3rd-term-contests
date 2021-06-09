#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>

enum { ARG_NUM = 2 };

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    int64_t min_number = LLONG_MAX;
    
    const char *filename = (const char *) argv[1];
    int fd = open(filename, O_RDWR);
    
    if (fd != -1) {
        int cur_index = 0;
        int offset_index = 0;

        while (1) {
            int64_t number;
            if (read(fd, &number, sizeof(number)) != sizeof(number)) {
                break;
            }
            
            if (number < min_number) {
                min_number = number;
                offset_index = cur_index;
            }   
            cur_index++;
        }

        if (min_number != LLONG_MIN && cur_index != 0) {
            min_number = -min_number;
            if (lseek(fd, offset_index * sizeof(min_number), SEEK_SET) == -1 ||
                    write(fd, &min_number, sizeof(min_number)) != sizeof(min_number)) {
                return 1;
            }
        }
        close(fd);
    }

    return 0;
}
