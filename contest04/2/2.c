#include <stdio.h>
#include <string.h>

const char sample[] = "rwxrwxrwx";

enum
{
    LENGTH = strlen(sample),
    BASE = 8,
    BIT_NUM = LENGTH / 3
};

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        unsigned int num;
        sscanf(argv[i], "%o", &num);
        
        int index[LENGTH] = {0};
        int count = LENGTH - 1;
        
        while (num) {
            unsigned int digit = num % BASE;
            for (int j = 0; j < BIT_NUM; ++j) {
                index[count--] = digit & 1;
                digit >>= 1;
            }              
            num /= BASE;
        }

        for (int j = 0; j < LENGTH; ++j) {
            if (index[j]) {
                putchar(sample[j]);
            } else {
                putchar('-');
            }
        }
        putchar('\n');
    }
    return 0;
}
