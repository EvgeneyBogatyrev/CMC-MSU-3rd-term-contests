#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>


void
exec_file(int proc_ind, char *argv[])
{
    int fd = open(argv[proc_ind + 2], O_RDONLY);

    if (fd < 0) {
        _exit(1);
    }
            
    char filename[PATH_MAX];
    ssize_t len = read(fd, filename, sizeof(filename));
            
    filename[len - 1] = '\0';
    close(fd);

    execlp(filename, filename, NULL);
    _exit(1);
}

int
main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 1;
    }

    int N;
    sscanf(argv[1], "%d", &N);

    int success_start = 0;
    int success_end = 0;
    
    int proc_ind = 0;

    for (; proc_ind < N && proc_ind < argc - 2; ++proc_ind) {
        pid_t pid = fork();
        if (pid < 0) {
            continue;
        } else if (!pid) {
            exec_file(proc_ind, argv);           
        } else {
            success_start++;
        }
    }
    
    for (int i = 0; i < success_start; ++i) {
        int status;
        wait(&status);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            success_end++;
        }
    }

    for (; proc_ind < argc - 2; ++proc_ind) {
        pid_t pid = fork();
        if (pid < 0) {
            continue;
        } else if (!pid) {
            exec_file(proc_ind, argv);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                success_end++;
            }
        }
    }

    printf("%d\n", success_end);
    return 0;
}
