#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>

enum 
{
    ARG_NUM = 2,
    END_LEN = 4
};

const char ending[] = ".exe";

int
main(int argc, char **argv)
{
    if (argc < ARG_NUM) {
        return 1;
    }

    DIR *dir = opendir(argv[1]);
    struct dirent *cur;

    int number = 0;
    while ((cur = readdir(dir))) {
        struct stat buf;
        char path[PATH_MAX];
        sprintf(path, "%s/%s", argv[1], cur->d_name);

        if (!stat(path, &buf) && S_ISREG(buf.st_mode) && !access(path, X_OK) &&
                !strcmp(cur->d_name + strlen(cur->d_name) - END_LEN, ending)) {
            number++;
        }
    } 
    closedir(dir);

    printf("%d\n", number);
    return 0;
}
