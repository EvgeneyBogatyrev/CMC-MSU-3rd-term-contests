#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>


enum
{
    ARG_NUM = 6
};

void operation(int *data, int ind1, int ind2, int value);

void
work(int *data, int iter_count, int count, int semid)
{
    struct sembuf oper;
    oper.sem_flg = 0;
    for (int i = 0; i < iter_count; ++i) {
        int ind1 = rand() % count;
        int ind2 = rand() % count;
        int value = rand() % 10;

        int max_ind = ind1 > ind2 ? ind1 : ind2;
        int min_ind = ind1 < ind2 ? ind1 : ind2;

        oper.sem_num = max_ind;
        oper.sem_op = -1;
        if (semop(semid, &oper, 1) == -1) {
            _exit(0);
        }

        if (min_ind != max_ind) {
            oper.sem_num = min_ind;
            if (semop(semid, &oper, 1) == -1) {
                _exit(0);
            }
        }

        operation(data, ind1, ind2, value);

        oper.sem_num = max_ind;
        oper.sem_op = 1;
        if (semop(semid, &oper, 1) == -1) {
            _exit(0);
        }

        if (min_ind != max_ind) {
            oper.sem_num = min_ind;
            if (semop(semid, &oper, 1) == -1) {
                _exit(0);
            }
        }
    }     
}

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    int count, nproc, iter_count;
    key_t key;

    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);

    int semid = semget(key, count, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        perror(argv[0]);
        return 1;
    }

    int *p = mmap(NULL, count * sizeof(*p), PROT_READ | PROT_WRITE, 
            MAP_ANON | MAP_SHARED, -1, 0);

    unsigned short *vals = calloc(count, sizeof(*vals));
    for (int i = 0; i < count; ++i) {
        scanf("%d", &p[i]);
        vals[i] = 1;
    }
    semctl(semid, 0, SETALL, vals);
    free(vals);

    for (int i = 0; i < nproc; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror(argv[0]);
            return 1;
        } else if (!pid) {
            unsigned seed;
            sscanf(argv[i + 5], "%u", &seed);
            srand(seed);
            work(p, iter_count, count, semid);
            _exit(0);
        }
    }

    while (wait(NULL) > 0) {}
    
    for (int i = 0; i < count; ++i) {
        printf("%d ", p[i]);
    }
    printf("\n");
    
    semctl(semid, -1, IPC_RMID);
    munmap(p, sizeof(*p) * count);
    return 0;
}
