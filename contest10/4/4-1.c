#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>


void
kill_children(pid_t *children, int end)
{
    for (int i = 0; i < end; ++i) {
        kill(children[i], SIGKILL);
        waitpid(children[i], NULL, 0);
    }
}

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        return 0;
    }

    int pfd[2];

    pid_t *children = calloc(argc - 1, sizeof(*children));
    if (!children) {
        return 1;
    }

    int fin = 0;
    int fout = 1;

    for (int i = 0; i < argc - 1; ++i) {
        
        if (i != 0) {
            fin = pfd[0];
        } else {
            fin = 0;
        }

        if (i != argc - 2) {
            fout = pfd[1];
        } else {
            fout = 1;
        }
       
        if (i != argc - 2 && pipe2(pfd, O_CLOEXEC) < 0) {
            kill_children(children, i);
            free(children);
            return 1;
        }

        if ((children[i] = fork()) < 0) {
            kill_children(children, i);
            free(children);
            return 1;

        } else if (children[i] == 0) {
            dup2(fin, 0); 
            dup2(fout, 1);
            
            if (i != 0) {
                close(fin);
            }
            if (i != argc - 2) {
                close(fout);
            }

            execlp(argv[i + 1], argv[i + 1], NULL);
            perror(argv[i + 1]);
            _exit(1);
        }
        
        //if (dup2(pfd[0], 0) < 0) {
          //  kill_children(children, i);
           // free(children);
            //return 1;
        //}

        int close_data = 0;
        if (i != 0) {
            close_data += close(fin);
        }
        if (i != argc - 2) {
            close_data += close(fout);
        }
        
        if (close_data != 0) {
            kill_children(children, i);
            free(children);
            return 1;
        }
    }

    for (int i = 0; i < argc - 1; ++i) {
        wait(NULL);
    }
    free(children);
   
    return 0;
}
