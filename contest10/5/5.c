#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <fcntl.h>


void
print_number(int id, int number, int out)
{
    dprintf(out, "%d %d\n", id, number);
    fflush(stdout);
}

void
child_work(int id, int max_num, int out)
{
    int number;
    while (scanf("%d", &number) == 1) {
        if (number == max_num) {
            break;   
        }
        print_number(id, number, out);
        printf("%d\n", ++number);
        fflush(stdout);
    }
}

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        perror(argv[0]);
        return 1;
    }

    int max_num;
    sscanf(argv[1], "%d", &max_num);

    int pfd1[2];
    int pfd2[2];

    pipe(pfd1);
    pipe(pfd2);

    int out = dup(1);

    int pids[2];
    if (!(pids[0] = fork())) {
        dup2(pfd2[0], 0);
        dup2(pfd1[1], 1);

        close(pfd1[0]);
        close(pfd1[1]);
        close(pfd2[0]);
        close(pfd2[1]);

        child_work(1, max_num, out);
        _exit(0);

    } else if (pids[0] < 0) {
        perror(argv[0]);
        return 1;
    }

    if (!(pids[1] = fork())) {
        dup2(pfd1[0], 0);
        dup2(pfd2[1], 1);

        printf("%d\n", 1);
        fflush(stdout);

        close(pfd1[0]);
        close(pfd1[1]);
        close(pfd2[0]);
        close(pfd2[1]);

        child_work(2, max_num, out);
        _exit(0);
    } else if (pids[1] < 0) {
        perror(argv[0]);
        return 1;
    }

    close(pfd1[0]);
    close(pfd1[1]);
    close(pfd2[0]);
    close(pfd2[1]);
    
    wait(NULL);
    wait(NULL);

    printf("Done\n");
    fflush(stdout);

    return 0;
}
