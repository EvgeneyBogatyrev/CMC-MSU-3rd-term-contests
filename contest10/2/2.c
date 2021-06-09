#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>

int
main(void)
{
    int pfd[2];
    pipe(pfd);

    if (!fork()) {
        close(pfd[1]);
        if (!fork()) {
            int32_t data;
            int64_t sum = 0;
            while (read(pfd[0], &data, sizeof(data)) == sizeof(data)) {
                sum += data;
            }
            printf("%" PRId64 "\n", sum);
            fflush(stdout);
            close(pfd[0]);
            _exit(0);
        }
        wait(NULL);
        _exit(0);
    }

    int32_t input;
    while (scanf("%" SCNd32, &input) == 1) {
        write(pfd[1], &input, sizeof(input));
    }    
    close(pfd[1]);

    wait(NULL);
    close(pfd[0]);
    return 0;
}
