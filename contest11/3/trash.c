#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>


int
main(int argc, char *argv[])
{
    if (argc < 2) {
        perror(argv[0]);
        return 1;
    }
    int N;
    sscanf(argv[1], "%d", &N);

    if (argc < 2 + N) {
        perror(argv[0]);
        return 1;
    }

    int counter = 0;
    int index = 0;
    for (int i = 0; i < N; ++i) {
        pid_t proc = fork();
        if (!proc) {
            index = i + 1;
            break;
        } 
    }

    if (index) {
        int fd = open(argv[index + 1], O_RDONLY);
        
        char buffer[PATH_MAX];        
        read(fd, &buffer, sizeof(buffer));


        close(fd);

        execlp(buffer, buffer, NULL);
        _exit(1);
    } else {
        for (int i = 0; i < N; ++i) {
            int status;
            wait(&status);
            if (WEXITSTATUS(status) == 0) {
                counter++;
            }   
        }
    }

    for (int i = 2 + N; i < argc; ++i) { 
        int fd = open(argv[i], O_RDONLY);
        
        char buffer[PATH_MAX];        
        read(fd, &buffer, sizeof(buffer));

        close(fd);

        if (!fork()) {
            execlp(buffer, buffer, NULL);
            _exit(1);
        } else {
            int status;
            wait(&status);
            if (WEXITSTATUS(status) == 0) {
                counter++;
            }
        }
    }

    printf("%d\n", counter);

    return 0;
}
