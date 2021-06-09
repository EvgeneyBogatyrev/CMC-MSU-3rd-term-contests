#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

enum
{
    ARG_NUM = 4
};

int
run(char *str) {
    
    if (!fork()) {   
        execlp(str, str, NULL);
        _exit(1);
    }

    int status;
    wait(&status);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        return 1;
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    if ((run(argv[1]) || run(argv[2])) && run(argv[3])) {
        return 0;
    }

    return 1;
}
