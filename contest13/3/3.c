#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/types.h>  
#include <sys/ipc.h> 
#include <sys/msg.h>  
#include <inttypes.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/sem.h>

enum 
{
    ARG_NUM = 6,
    MSGSIZE = 128
};

struct Msgbuf 
{
    long mtype;
    char mtext[MSGSIZE];
};

void
kill_children(pid_t *pids, int index)
{
    for (int i = 0; i < index; ++i) {
        kill(pids[i], SIGKILL);
        waitpid(pids[i], NULL, 0);
    }
}

void
work(int semid, int msgid, int id, int64_t maxval, int n)
{
    struct Msgbuf buf;
    struct sembuf oper;
    oper.sem_flg = 0;

    while (1) {
        oper.sem_num = id;
        oper.sem_op = -1;
        if (semop(semid, &oper, 1) == -1) {
            _exit(0);
        }

        if (msgrcv(msgid, &buf, MSGSIZE, 1, 0) < 0) {
            _exit(1);
        }

        int64_t val1, val2;
        sscanf(buf.mtext, "%" SCNd64 " %" SCNd64, &val1, &val2);

        int64_t sum = val1 + val2;

        printf("%d %" PRId64 "\n", id, sum);
        fflush(stdout);

        if (sum > maxval) {
            semctl(semid, 0, IPC_RMID);
            msgctl(msgid, 0, IPC_RMID);
            _exit(0);
        }

        memset(buf.mtext, 0, MSGSIZE);
        sprintf(buf.mtext, "%" PRId64 " %" PRId64 "\n", val2, sum);

        buf.mtype = 1;

        if (msgsnd(msgid, &buf, MSGSIZE, 0) < 0) {
            _exit(1);
        }
        
        oper.sem_num = (int) (sum % n);
        oper.sem_op = 1;
        if (semop(semid, &oper, 1) == -1) {
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

    key_t key;
    int n;
    int64_t value1, value2, maxval;

    sscanf(argv[1], "%d", &key);
    sscanf(argv[2], "%d", &n);
    sscanf(argv[3], "%" SCNd64, &value1);
    sscanf(argv[4], "%" SCNd64, &value2);
    sscanf(argv[5], "%" SCNd64, &maxval);

    int msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
    if (msgid < 0) {
        perror(argv[0]);
        return 1;
    }

    int semid = semget(key, n, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid < 0) {
        perror(argv[0]);
        msgctl(msgid, 0, IPC_RMID);
        return 1;
    }

    pid_t *pids = calloc(n, sizeof(*pids));
    for (int i = 0; i < n; ++i) {
        pids[i] = fork();
        if (pids[i] < 0) {
            kill_children(pids, i);
            free(pids);
            semctl(semid, 0, IPC_RMID);
            msgctl(msgid, 0, IPC_RMID);
            return 1;
        } else if (!pids[i]) {
            work(semid, msgid, i, maxval, n);
            _exit(0);
        }
    }

    struct Msgbuf buf;
    buf.mtype = 1;
    sprintf(buf.mtext, "%" PRId64 " %" PRId64 "\n", value1, value2);

    if (msgsnd(msgid, &buf, MSGSIZE, 0) < 0) {
        kill_children(pids, n);
        free(pids);
        semctl(semid, 0, IPC_RMID);
        msgctl(msgid, 0, IPC_RMID);
        return 1;
    }

    struct sembuf oper;
    oper.sem_flg = 0;
    oper.sem_num = 0;
    oper.sem_op = 1;
    if (semop(semid, &oper, 1) < 0) {
        free(pids);
        semctl(semid, 0, IPC_RMID);
        msgctl(msgid, 0, IPC_RMID);
        return 1;
    }

    while (wait(NULL) > 0) {}
    free(pids);
    return 0;
}
