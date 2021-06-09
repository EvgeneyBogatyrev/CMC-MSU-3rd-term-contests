#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include <sys/sem.h>


enum
{
    ARG_NUM = 4,
    EXIT = 2
};

struct Data
{
    int num;
    int id;
};


void
child_work(int semid, struct Data *p, int id, int maxval, int nproc)
{
    struct sembuf ops;
    ops.sem_flg = 0;
   
    while (1) {
        ops.sem_num = id;
        ops.sem_op = -1;
        if (semop(semid, &ops, 1) == -1) {
            _exit(0);
        }
        
        printf("%d %d %d\n", id, p->num, p->id);
        fflush(stdout);
        
        p->num += 1;
        p->id = id;

        if (p->num > maxval) {
            semctl(semid, -1, IPC_RMID);
            _exit(0);
        }

        uint64_t value = (uint64_t) p->num;
        for (int i = 0; i < 3; ++i) {
            value *= p->num;
            value %= nproc;
        }
        ++value;
        int send_to = (int) value;
    
        ops.sem_num = send_to;
        ops.sem_op = 1;
        if (semop(semid, &ops, 1) == -1) {
            _exit(0);
        }
    }
}

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    struct Data *p = mmap(NULL, sizeof(*p), PROT_WRITE | PROT_READ, MAP_ANON | MAP_SHARED, -1, 0);

    int nproc, maxval;
    key_t key;
    sscanf(argv[1], "%d", &nproc);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &maxval);    

    int semid = semget(key, nproc + 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        perror(argv[0]);
        return 1;
    }
   


    pid_t *pids = calloc(nproc, sizeof(*pids));

    int id = 0;
    for (int i = 0; i < nproc; ++i) {
        if ((pids[i] = fork()) < 0) {
            perror(argv[0]);
            return 1;
        } else if (!pids[i]) {
            id = i + 1;
            break;
        }
    }

    if (id != 0) {
        child_work(semid, p, id, maxval, nproc);
        _exit(1);
    }

    p->num = 0;
    p->id = 0;
    semctl(semid, 1, SETVAL, 1);
 
    while (wait(NULL) > 0) {}

    free(pids);
    munmap(p, sizeof(*p));
    return 0;
}
