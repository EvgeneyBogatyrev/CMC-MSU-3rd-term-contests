#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
    int fd = open(argv[1], O_CREAT | O_WRONLY, 0660);

    

    fclose(fd);
}
