#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

char *pr = "#include <stdio.h>\n #define summon \"summon\"\n#define disqualify \"disqualify\"\n#define reject \"reject\"\nint main(void) {\nint x;\nwhile (scanf(\"%%d\", &x) == 1) {\nprintf(\"%%s\\n\", (%s));}}";

int
main(int argc, char *argv[])
{
    int fd = open("prog.c", O_CREAT | O_WRONLY, 0666);

    write(fd, pr, sizeof(pr));
    close(fd);

    system("gcc prog.c -o p");
    system("./p");

    return 0;
}
