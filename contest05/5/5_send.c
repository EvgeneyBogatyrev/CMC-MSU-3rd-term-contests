#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    USER_SHIFT = 6,
    GROUP_SHIFT = 3,
};

int myaccess(const struct stat *stb, const struct Task *task, int access);
static int check_access_user(unsigned int mode, unsigned int access);
static int check_access_group(unsigned int mode, unsigned int access);
static int check_access_others(unsigned int mode, unsigned int access);

static int
check_access_user(unsigned int mode, unsigned int access)
{
    access <<= USER_SHIFT;
    return (mode & access) == access;
}

static int
check_access_group(unsigned int mode, unsigned int access)
{
    access <<= GROUP_SHIFT;
    return (mode & access) == access;
}

static int
check_access_others(unsigned int mode, unsigned int access)
{
    return (mode & access) == access;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (task->uid == 0) {
        return 1;
    }

    unsigned int tmp = (unsigned int) access; 

    if (stb->st_uid == task->uid) {
        return check_access_user(stb->st_mode, tmp);
    }

    for (int i = 0; i < task->gid_count; ++i) {
        if (task->gids[i] == stb->st_gid) {
            return check_access_group(stb->st_mode, tmp);
        }
    }

    return check_access_others(stb->st_mode, tmp);
}
