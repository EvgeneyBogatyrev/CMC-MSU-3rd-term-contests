#include <stdio.h>
#include <string.h>
#include <ctype.h>

int
main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        if (len == 0) {
            continue;
        }

        int flag = 1;
        for (int start = 0, end = len -1; start < end; ++start, --end) {
            if (!islower(argv[i][start]) || argv[i][start] != argv[i][end]) {
                flag = 0;
                break;
            }
        }
        if (flag) {
            printf("%s\n", argv[i]);
        }
    }
    return 0;
}
