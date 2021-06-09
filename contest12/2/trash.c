#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <inttypes.h>

enum { ARG_NUM = 4 };

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    uint32_t count;
    uint32_t modulo;

    sscanf(argv[1], "%" SCNu32, &count);
    sscanf(argv[2], "%" SCNu32, &modulo);

    int pfd2[2];
    pipe(pfd2);

    if (!fork()) {
        //P1
        int pfd[2];
        pipe(pfd);

        if (!fork()) {
            //P2
            dup2(pfd[0], 0);
            dup2(pfd2[1], 1);

            close(pfd[1]);
            close(pfd[0]);
            close(pfd2[0]);
            close(pfd2[1]);

            execlp(argv[3], argv[3], NULL);
            perror(argv[3]);
            _exit(1);
        }

        
        close(pfd[0]);

        close(pfd2[0]);
        close(pfd2[1]);

        for (uint32_t i = 1; i <= count; ++i) {
            dprintf(pfd[1], "%" PRIu32 " ", (i * i) % modulo);
//            fflush(stdout);
        }
        close(pfd[1]);
        wait(NULL);
        _exit(0);
    }

    if (!fork()) {
        //P3
        if (!fork()) {
            //P4
            
            close(pfd2[1]);
            
            char buf[2048];
            while ((read(pfd2[0], &buf, sizeof(buf))) > 0) {
                for (int i = 0; i < 2048; ++i) {
                    if (buf[i] == ' ') {
                        buf[i] = '\n';
                    }
                }
                
                printf("%s", buf);
            }
           
            close(pfd2[0]); 
            _exit(0);
        }

        close(pfd2[0]);
        close(pfd2[1]);
        wait(NULL);
        _exit(0);
    }
    close(pfd2[0]);
    close(pfd2[1]);

    wait(NULL);
    wait(NULL);
    printf("0\n");

    return 0;
}
