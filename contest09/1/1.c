#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

enum
{
    ARG_NUM = 5,
    RETURN_CODE = 42
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    if (!fork()) {
        int fdin = open(argv[2], O_RDONLY);
        if (fdin < 0) {
            _exit(RETURN_CODE);
        }
        int fdout = open(argv[3], O_CREAT | O_WRONLY | O_APPEND, 0660);
        if (fdout < 0) {
            _exit(RETURN_CODE);
        }
        
        int fderr = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0660);
        if (fdout < 0) {
            _exit(RETURN_CODE);
        }

        if (dup2(fdin, 0) == -1) {
            _exit(RETURN_CODE);
        }
        if (dup2(fdout, 1) == -1) {
            _exit(RETURN_CODE);
        }
        if (dup2(fderr, 2) == -1) {
            _exit(RETURN_CODE);
        }

        if (close(fdin)) {
            _exit(RETURN_CODE);
        }
        if (close(fdout)) {
            _exit(RETURN_CODE);
        }
        if (close(fderr)) {
            _exit(RETURN_CODE);
        }

        execlp(argv[1], argv[1], NULL);
        _exit(RETURN_CODE);
    }

    pid_t status;
    wait(&status);

    printf("%d\n", status);
    return 0;
}
