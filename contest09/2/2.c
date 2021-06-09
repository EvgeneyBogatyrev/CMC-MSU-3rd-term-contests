#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum 
{
    MAX_CODE = 128
};

int mysys(const char *str);

int
mysys(const char *str)
{
    int pid;
    if (!(pid = fork())) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(MAX_CODE - 1);
        return MAX_CODE - 1;
    } else if (pid < 0) {
        return -1;
    }

    int status;
    waitpid(pid, &status, 0);    
    
    if (WIFSIGNALED(status)) {
        return MAX_CODE + WTERMSIG(status);
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return -1;
}
