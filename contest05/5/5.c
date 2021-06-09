#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    USER_MASK = 0700,
    GROUP_MASK = 0070,
    OTHERS_MASK = 0007,
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
        if ((access & 1 == 1) && (mode & 1 == 0)) {
            return 0;
        }
        access >>= 1;
        mode >>= 1;
    }
    return 1;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (task.uid == 0) {
        return 1;
    }

    if (stb.st_uid == task.uid) {
        mode_t mode = stb.st_mode & USER_MASK;
        mode >>= USER_SHIFT;
        return check_access(mode, access);
    }

    for (int i = 0; i < task.gid_count; ++i) {
        if (task.gids[i] == stb.st_gid) {
            mode_t mode = stb.st_mode & GROUP_MASK;
            mode >>= GROUP_SHIFT;
            return check_access(mode, access);
        }
    }

    mode_t mode = stb.st_mode & OTHERS_MASK;
    return check_access(mode, access);
}
