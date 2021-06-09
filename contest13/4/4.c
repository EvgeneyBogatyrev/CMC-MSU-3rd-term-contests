#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <inttypes.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

enum { INIT_SIZE = 8 };

void
work(int semid, int id, int count, int32_t *data, int n, int *index)
{
    struct sembuf oper;
    oper.sem_flg = 0;
    while (1) {
        oper.sem_num = id;
        oper.sem_op = -1;
        if (semop(semid, &oper, 1) == -1) {
            _exit(0);
        }

        if (*index == n) {
            semctl(semid, 0, IPC_RMID);
            _exit(0);
        }

        int32_t num = data[*index];
        (*index)++;

        printf("%d %" PRId32 "\n", id, num);
        fflush(stdout);

        int remainder = num % count;
        if (remainder < 0) {
            remainder += count;
        }
        oper.sem_num = remainder;
        oper.sem_op = 1;
        if (semop(semid, &oper, 1) == -1) {
            _exit(0);
        }
    }
}


int
main(int argc, char *argv[])
{
    if (argc < 2) {
        return 1;
    }

    int count;
    sscanf(argv[1], "%d", &count);

    srand(time(NULL));
    int proj_id = rand();
    key_t key = ftok(argv[0], proj_id);

    int semid = semget(key, count, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        perror(argv[0]);
        return 1;
    }

    int32_t *input = calloc(INIT_SIZE, sizeof(*input));
    int cnt = 0;
    int max_size = INIT_SIZE;

    while (scanf("%" SCNd32, &input[cnt]) == 1) {
        cnt++;
        if (cnt == max_size) {
            max_size += max_size;
            input = realloc(input, max_size * sizeof(*input));
        }
    }

    int *index = mmap(NULL, sizeof(*index), PROT_READ | PROT_WRITE, 
            MAP_ANON | MAP_SHARED, -1, 0);
    *index = 0;
    
    for (int i = 0; i < count; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            return 1;
        } else if (!pid) {
            work(semid, i, count, input, cnt, index);
            _exit(0);
        }
    }

    semctl(semid, 0, SETVAL, 1);

    while (wait(NULL) > 0) {}
    munmap(index, sizeof(*index));
    free(input);
    return 0;
}
