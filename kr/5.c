#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

enum
{
    ARG_NUM = 3
};

void
trav(char *path, char *rel_path, int depth, unsigned int max_size)
{
    if (depth == 0) {
        return;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        return;
    }
    
    struct dirent *dir_info;
    while ((dir_info = readdir(dir))) {
        char cur_path[PATH_MAX];
        char cur_rel_path[PATH_MAX];
        snprintf(cur_path, PATH_MAX, "%s/%s", path, dir_info->d_name);
        if (depth < 4) {
            snprintf(cur_rel_path, PATH_MAX, "%s/%s", rel_path, dir_info->d_name);
        } else {
            snprintf(cur_rel_path, PATH_MAX, "%s", dir_info->d_name);
        }
        if (!strcmp(dir_info->d_name, ".") || !strcmp(dir_info->d_name, "..")) {
            continue;
        }

        struct stat buf;
        if (stat(cur_path, &buf)) {
            continue;
        }
        if (S_ISREG(buf.st_mode) && !access(cur_path, R_OK) && buf.st_size <= max_size) {
            printf("%s\n", cur_rel_path);
        }
    }
    closedir(dir);

    dir = opendir(path);
    while ((dir_info = readdir(dir))) {
        char cur_path[PATH_MAX];
        char cur_rel_path[PATH_MAX];
        snprintf(cur_path, PATH_MAX, "%s/%s", path, dir_info->d_name);
        if (depth < 4) {
            snprintf(cur_rel_path, PATH_MAX, "%s/%s", rel_path, dir_info->d_name);
        } else {
            snprintf(cur_rel_path, PATH_MAX, "%s", dir_info->d_name);
        }
        if (!strcmp(dir_info->d_name, ".") || !strcmp(dir_info->d_name, "..")) {
            continue;
        }

        struct stat buf;
        if (stat(cur_path, &buf)) {
            continue;
        }
 
        if (S_ISDIR(buf.st_mode)) {
            trav(cur_path, cur_rel_path, depth - 1, max_size);
        }
    }

    closedir(dir);
}

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        return 1;
    }

    unsigned int max_size;
    sscanf(argv[2], "%u", &max_size);

    trav(argv[1], "", 4, max_size);
    return 0;
}
