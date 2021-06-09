#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int
main(void)
{
    int N;
    scanf("%d", &N);

    printf("1");
    if (N == 1) {
        printf("\n");
    } else {
        printf(" ");
    }
    fflush(stdout);
    int process_created = 0;
    for (int i = 1; i < N; ++i) {
        if (!process_created && !fork()) {
            process_created = 0;
            printf("%d", i + 1);
            if (i == N - 1) {
                printf("\n");
            } else {
                printf(" ");
            }
            fflush(stdout);
        } else {
            process_created = 1;
            wait(NULL);
            _exit(0);
        }
    }

    return 0;
}
