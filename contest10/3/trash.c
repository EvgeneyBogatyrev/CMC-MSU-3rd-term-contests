#include <unistd.h>
#include <sys/wait.h>


int
main(int argc, char *argv)
{
    if (argc < 6) {
        return 1;
    }

    if (!fork()) {
        if (!fork()) {
            if fin = fopen(argv[4], O_RDONLY);
            dup2(fin, 0);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == 0) {
            if (!fork())
        }
    }

    return 0;
}
