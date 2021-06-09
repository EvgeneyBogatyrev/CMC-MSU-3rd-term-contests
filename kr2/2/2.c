#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>

int
main(int argc, char *argv[])
{
    int64_t num;
    int pfd[argc - 1][2];
    for (int i = 1; i < argc; ++i) {
        pipe(pfd[i - 1]);
        if (!fork()) {
            sscanf(argv[i], "%" SCNd64, &num);

            if (num < 0) {
                num = -num;
            }
            
            int sum = 0;

            while (num > 0) {
                sum += num % 4;
                num /= 4;
            }

            close(pfd[i - 1][0]);
            write(pfd[i - 1][1], &sum, sizeof(sum));
            close(pfd[i - 1][1]);
            _exit(0);
        }
        close(pfd[i - 1][1]);
    }


    for (int i = 1; i < argc; ++i) {
        int sum;
        read(pfd[i - 1][0], &sum, sizeof(sum));
        close(pfd[i - 1][0]);
        printf("%d\n", sum);
    }

    while (wait(NULL) > 0) {}
    
    return 0;
}
