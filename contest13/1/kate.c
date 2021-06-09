#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <sys/mman.h>
#include <string.h>

void 
play(int semid, int64_t *x, int num, int32_t maxval, int nproc, int *prev) 
{
    while (1) {
        struct sembuf r_b[] = 
        {
            { .sem_num = num, -1, 0 },
        };
        int sem = semop(semid, r_b, sizeof(r_b) / sizeof(r_b[0]));
        if (sem == -1) {
            _exit(0);
        }
        printf("%d %d %d\n", num, (int)*x, (int)*prev);
        fflush(stdout);
        
        ++(*x);
        uint32_t val = (uint32_t)*x;
        if (val > maxval) {
            semctl(semid, 0, IPC_RMID);
            _exit(0);
        }
        val = (uint32_t)*x;
        uint64_t calc_val = 1;
        for (int k = 0; k < 4; ++k) {
            calc_val *= (val % nproc);
            calc_val %= nproc;
        }
        calc_val += 1;
        int next_num = (int)calc_val;
        *(prev) = num;

        struct sembuf w_b[] = 
        {
            { .sem_num = next_num, 1, 0 },
        };
        sem = semop(semid, w_b, sizeof(w_b) / sizeof(w_b[0]));
        if (sem == -1) {
            _exit(0);
        }
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        return 0;
    }
    key_t key;
    int32_t nproc, maxval;
    nproc = (int32_t) strtol(argv[1], NULL, 10);
    key = (key_t) strtol(argv[2], NULL, 10);
    maxval = (int32_t) strtol(argv[3], NULL, 10);
    int64_t *x = (int64_t *)mmap(NULL, sizeof(*x), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    int *prev = (int *) mmap(NULL, sizeof(*prev), PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
    memset(x, 0, sizeof(*x));
    memset(prev, 0, sizeof(*prev));
    int semid = semget(key, nproc+1, 0600 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        return 0;
    }
    semctl(semid, 1, SETVAL, 1);
    for (int i = 0; i < nproc; ++i) {
        int pid = fork();
        if (pid < 0) {
            return 0;
        }
        if (!pid) {
            play(semid, x, i + 1, maxval, nproc, prev);
        }
    }
    while (wait(NULL) > 0) {};
    munmap(x, sizeof(*x));
    munmap(prev, sizeof(prev));
    return 0;
}
