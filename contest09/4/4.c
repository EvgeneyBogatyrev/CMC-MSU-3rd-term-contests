#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    MODE = 0777
};

const char *runtime = "XDG_RUNTIME_DIR";
const char *tmpdir = "TMPDIR";
const char *tmp_path = "/tmp";

const char *script = "#! /usr/bin/env python3\nprint(";

int
main(int argc, char *argv[])
{
    char *folder = getenv(runtime);
    if (!folder) {
        folder = getenv(tmpdir);
    }
    if (!folder) {
        folder = (char *) tmp_path;
    }


    pid_t pid = getpid();
    char path[PATH_MAX];
    char name[PATH_MAX];

    int len = snprintf(name, PATH_MAX, "%u.py", pid);
    if (len < 0) {
        return 1;
    }

    len = snprintf(path, PATH_MAX, "%s/%s", folder, name);    
    if (len < 0) {
        return 1;
    }
 
    int fd = open(path, O_CREAT | O_EXCL | O_WRONLY, MODE);
    if (fd < 0) {
        return 1;
    }
    
    write(fd, script, strlen(script)); 
    for (int i = 1; i < argc; ++i) {
        write(fd, argv[i], strlen(argv[i]));
        if (i != argc - 1) {
            write(fd, "*", 1);
        } else {
            write(fd, ")\n", 2);
        }
    }
    close(fd);

    execlp(path, name, NULL);
    perror(argv[0]);
    return 0;
}
