#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

enum 
{
    SUCCESS = 0,
    ERROR = 1,
    ARG_NUM = 2,
    OFFSET = 4,
    FIELD_NUM = 3,
    LEFT = 1,
    RIGHT = 2,
    NODE_LEN = FIELD_NUM * OFFSET
};

int
print_tree(int fd, int32_t root_offset) 
{
    int32_t buffer[FIELD_NUM];

    if (lseek(fd, root_offset, SEEK_SET) == -1) {
        return ERROR;
    }   

    if (read(fd, &buffer, sizeof(buffer)) != sizeof(buffer)) {
        return ERROR;
    }

    if (buffer[RIGHT] != 0) {
        if (print_tree(fd, buffer[RIGHT] * NODE_LEN) == ERROR) {
            return ERROR;
        }
    }

    printf("%" PRId32 " ", buffer[0]);

    if (buffer[LEFT] != 0) {
        if (print_tree(fd, buffer[LEFT] * NODE_LEN) == ERROR) {
            return ERROR;
        }
    }

    return SUCCESS;
}

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    
    if (fd == -1) {
        fprintf(stderr, "Cannot open\n");
        return 1;
    }

    if (print_tree(fd, 0) == ERROR) {
        close(fd);
        fprintf(stderr, "Cannot read\n");
        return 1;
    }
    close(fd);

    printf("\n");
    return 0;
}
