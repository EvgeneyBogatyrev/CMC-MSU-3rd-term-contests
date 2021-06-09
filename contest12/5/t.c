#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    char o = 6;
    write(fd, &o, sizeof(o));
    close(fd);
    return 0;
}
