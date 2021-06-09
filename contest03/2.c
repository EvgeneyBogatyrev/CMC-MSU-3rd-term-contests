#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <unistd.h>

enum { BYTE_LEN = 5 };

void
print(int32_t number)
{
    int64_t result = (int64_t) number;

    if (result < 0) {
        printf("-");
        result *= -1;
    }
    printf("%" PRId64 ".", result / 100);
    
    int64_t remainder = result % 100;
    if (remainder == 0) {
        printf("00\n");
    } else if (remainder < 10) {
        printf("0%" PRId64 "\n", remainder);
    } else {
        printf("%" PRId64 "\n", remainder);
    }
}

int
main(int argc, char *argv[])
{
    int32_t greatest = 0;
    int32_t second = 0;

    int init_greatest = 0;
    int init_second = 0;

    for (int i = 1; i < argc; ++i) {
        const char *filename = (const char *) argv[i];
        int fd = open(filename, O_RDONLY);

        if (fd != -1) {
            while (1) {
                int32_t buffer[BYTE_LEN];
                
                int read_len = read(fd, buffer, sizeof(buffer));
                if (read_len <= 0) {
                    break;
                }
                
                int32_t price = buffer[BYTE_LEN - 1];
                
                if (!init_greatest || price > greatest) {
                    if (init_greatest) {
                        second = greatest;
                        init_second = 1;
                    }
                    greatest = price;
                    init_greatest = 1;
                } else if (price < greatest && (!init_second || price > second)) {
                    second = price;
                    init_second = 1;
                }

            }
            close(fd);
        }
    }    

    if (init_second) {
        print(second);
    } else {
        printf("\n");
    }    
    return 0;
}
