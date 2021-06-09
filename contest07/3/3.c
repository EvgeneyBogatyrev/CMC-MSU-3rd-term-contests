#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <inttypes.h>

enum
{
    ARG_NUM = 4,
    MODE = 0777,
    ROWS_IND = 2,
    COLS_IND = 3
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, MODE);

    if (fd <= 0) {
        fprintf(stderr, "Cannot open\n");
        return 1;
    }

    int rows;
    if (sscanf(argv[ROWS_IND], "%d", &rows) != 1) {
        fprintf(stderr, "Cannot read rows\n");
        return 1;
    }

    int cols;
    if (sscanf(argv[COLS_IND], "%d", &cols) != 1) {
        fprintf(stderr, "Cannot read cols\n");
        return 1;
    }

    int new_file_size = rows * cols * sizeof(int32_t);
    if (ftruncate(fd, new_file_size) == -1) {
        fprintf(stderr, "ftruncate error\n");
        return 1;
    }

    int32_t *p = mmap(NULL, new_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);   
    close(fd);

    if (p == MAP_FAILED) {
        fprintf(stderr, "Mapping failed\n");
        return -1;
    }

    int32_t counter = 0;
    
    int top = 0;
    int bottom = rows - 1;
    int left = 0;
    int right = cols - 1;

    while (counter < rows * cols) {
        for (int x = left; x <= right && counter < rows * cols; ++x) {
            *(p + top * cols + x) = ++counter;
        }

        for (int y = top + 1; y <= bottom && counter < rows * cols; ++y) {
            *(p + y * cols + right) = ++counter;
        }
        
        for (int x = right - 1; x >= left && counter < rows * cols; --x) {
            *(p + bottom * cols + x) = ++counter;
        }

        for (int y = bottom - 1; y > top && counter < rows * cols; --y) {
            *(p + y * cols + left) = ++counter;
        }

        top++;
        left++;
        right--;
        bottom--;
    }

    munmap(p, new_file_size);
    return 0;
}
