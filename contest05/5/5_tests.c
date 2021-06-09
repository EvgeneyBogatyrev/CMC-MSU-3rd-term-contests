#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    MASK = 0007,
    USER_SHIFT = 6,
    GROUP_SHIFT = 3,
    MODE_LEN = 3
};


int myaccess(const struct stat *stb, const struct Task *task, int access);
int check_access(mode_t mode, int access);

int
check_access(mode_t mode, int access)
{
    for (int i = 0; i < MODE_LEN; ++i) {
        int mask = 1u << i;
        
        if ((access & mask) == 1 && (mode & 1) == 0) {
            return 0;
        }
    }
    return 1;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (task->uid == 0) {
        return 1;
    }

    if (stb->st_uid == task->uid) {
        mode_t mode = stb->st_mode >> USER_SHIFT;
        mode &= MASK;
        return check_access(mode, access);
    }

    for (int i = 0; i < task->gid_count; ++i) {
        if (task->gids[i] == stb->st_gid) {
            mode_t mode = stb->st_mode >> GROUP_SHIFT;
            mode >>= MASK;
            return check_access(mode, access);
        }
    }

    mode_t mode = stb->st_mode & MASK;
    return check_access(mode, access);
}

int
main(void)
{
    struct Task task = {.uid = 0};
    struct stat st;
    stat("test", &st);
    printf("%d\n", myaccess(&st, &task, 213));
    return 0;
}
