#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

enum 
{ 
    INIT_LEN = 1024,
    DIGITS_NUM = 10,
    BUFFER_LEN = 4096
};

int
main(void)
{
    char *buffer = calloc(INIT_LEN, 1);
    size_t max_len = INIT_LEN;

    int len = getline(&buffer, &max_len, stdin);

    while (len && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
        buffer[--len] = '\0';
    }

    uint64_t digits[DIGITS_NUM] = { 0 }; 

    const char *filename = (const char *) buffer;
    int fd = open(filename, O_RDONLY);    
    free(buffer);

    if (fd != -1) {
        unsigned char symbols[BUFFER_LEN];
        while (1) {
            int len = read(fd, symbols, sizeof(symbols));

            if (len <= 0) {
                break;
            }

            for (int i = 0; i < len; ++i) {
                if (isdigit(symbols[i])) {
                    digits[symbols[i] - '0']++;
                }
            }
        }
        close(fd);
    }

    for (int i = 0; i < DIGITS_NUM; ++i) {
        printf("%d %" PRIu64 "\n", i, digits[i]);
    }    
    return 0;
}
