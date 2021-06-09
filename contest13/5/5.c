#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>
#include <time.h>

enum
{
    MSGSIZE = 4096,
    MAXNUM = 2048,
    MODE = 0666
};

struct Msgbuf 
{
    long mtype;
    char mtext[MSGSIZE];
};


void
work(int semid, int msgid, int out, int id, int type1, int type2)
{
    struct Msgbuf buf1, buf2;
    struct sembuf oper;
    oper.sem_flg = 0;
    oper.sem_op = -1;
    oper.sem_num = id - 1;
    if (semop(semid, &oper, 1) == -1) {
        _exit(0);
    }

    msgrcv(msgid, &buf1, MSGSIZE, type1, 0);
    
    if (type2 < 0) {
        buf1.mtype = id;
        msgsnd(out, &buf1, MSGSIZE, 0);
        _exit(0);
    }
    
    msgrcv(msgid, &buf2, MSGSIZE, type2, 0);  

    int skip[2] = {0};
    int num1, num2;

    char *p1 = buf1.mtext;
    char *p2 = buf2.mtext;

    char *p1st = p1;
    char *p2st = p2;

    char *tmp;

    char buffer[MSGSIZE];

    while (1) {
        
        if (!skip[0]) {
            tmp = p1;
            if (tmp - p1st >= strlen(buf1.mtext)) {
                _exit(0);
            }
            while ((tmp - p1st) < strlen(buf1.mtext) && *tmp != ' ') {
                tmp++;
            }
            
            *tmp = '\0';
            sscanf(p1, "%d", &num1);
            p1 = tmp + 1;
        }
        skip[0] = 0;

        if (!skip[1]) {
            tmp = p2;
            if (tmp - p2st >= strlen(buf2.mtext)) {
                _exit(0);
            }

            while ((tmp - p2st) < strlen(buf2.mtext) && *tmp != ' ') {
                tmp++;
            }

            *tmp = '\0';
            sscanf(p2, "%d", &num2);
            p2 = tmp + 1;
        }
        skip[1] = 0;

        if (num1 == num2) {
            sprintf(buffer, "%d ", num1);
        } else if (num1 > num2) {
            skip[0] = 1;
        } else if (num1 < num2) {
            skip[1] = 1;
        }
    }

    memset(buf1.mtext, 0, MSGSIZE);
    strcpy(buf1.mtext, buffer);
    buf1.mtype = id;
    msgsnd(out, &buf1.mtext, MSGSIZE, 0);
}

int
main(int argc, char *argv[])
{
    srand(time(NULL));
    key_t key = ftok(argv[0], rand() % MAXNUM);

    int init = 1;
    int odd = (argc - 1) & 1;
    int proc_num = (argc - 1) / 2 + odd;

    int msgid_in, msgid_out;

    while (1) {

        if (init) {
            init = 0;
        
            msgid_in = msgget(key, MODE | IPC_CREAT | IPC_EXCL);

            struct Msgbuf buf;
            for (int i = 1; i < argc; ++i) {
                int fd = open(argv[i], O_RDONLY);
                if (fd < 0) {
                    perror(argv[i]);
                    return 1;
                }
                FILE *ffd = fdopen(fd, "r");
                int num;

                buf.mtype = i;
                memset(buf.mtext, 0, MSGSIZE);

                while ((fscanf(ffd, "%d", &num)) == 1) {
                    sprintf(buf.mtext, "%d ", num);
                }
                msgsnd(msgid_in, &buf, MSGSIZE, 0);
                
                fclose(ffd);
            }

        } else {
            msgid_in = msgid_out;
        }

        key_t other_k = ftok(argv[0], rand() % MAXNUM);
        key_t yet_another = ftok(argv[0], rand() % MAXNUM);
        msgid_out = msgget(other_k, MODE | IPC_CREAT | IPC_EXCL);

        int semid = semget(yet_another, proc_num, MODE | IPC_CREAT | IPC_EXCL);
        
        int file_counter = 1;
        for (int i = 0; i < proc_num; ++i) {
            if (!fork()) {
                int second;
                if (i == proc_num - 1 && odd) {
                    second = -1;
                } else {
                    second = file_counter + 1;
                }
                work(semid, msgid_in, msgid_out, i + 1, file_counter, second);
                _exit(0);
            }
            file_counter += 2;

            struct sembuf oper;
            oper.sem_num = i;
            oper.sem_op = 1;
            oper.sem_flg = 0;

            semop(semid, &oper, 1);
        }

        odd = proc_num & 1;
        proc_num = proc_num / 2 + odd;
  
        while (wait(NULL) > 0) {}
        msgctl(msgid_in, 0, IPC_RMID);
        semctl(semid, 0, IPC_RMID);

        if (proc_num == 1 && odd) {
            break;
        }
    }

    struct Msgbuf buf;
    msgrcv(msgid_out, &buf, MSGSIZE, 1, 0);
    printf("%s\n", buf.mtext);

    msgctl(msgid_out, 0, IPC_RMID);
    return 0;
}
