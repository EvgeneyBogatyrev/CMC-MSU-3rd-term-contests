#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

enum
{
    ARG_NUM = 2,
    INIT_SIZE = 4
};

int compare(const void *s1, const void *s2)
{
    return strcasecmp(*((const char **) s1), *((const char **) s2));
}

int
explore_directory(DIR *directory, char *path)
{
    struct dirent *direct_info;

    int size = INIT_SIZE;
    char **filenames = calloc(size, sizeof(*filenames));
    if (!filenames) {
        return 1;
    }

    int counter = 0;

    while ((direct_info = readdir(directory))) {

        if (!strcmp(direct_info->d_name, ".") || !strcmp(direct_info->d_name, "..")) {
            continue;
        }

        if (strlen(path) + 1 + strlen(direct_info->d_name) >= PATH_MAX) {
            continue;
        }

        char cur_path[PATH_MAX];
        snprintf(cur_path, PATH_MAX, "%s/%s", path, direct_info->d_name);

        struct stat buf;
        if (lstat(cur_path, &buf) || !S_ISDIR(buf.st_mode)) {
            continue;
        }

        filenames[counter] = calloc(strlen(direct_info->d_name) + 1, 1);
        if (!filenames[counter]) {
            return 1;
        }
        strcpy(filenames[counter], (const char *) direct_info->d_name);
        counter++;
        if (counter == size) {
            size += size;
            char **q = realloc(filenames, size * sizeof(*filenames));
            if (!q) {
                return 1;
            }
            filenames = q;
        }
    }
    closedir(directory);
   
    qsort(filenames, counter, sizeof(*filenames), compare);

    for (int i = 0; i < counter; ++i) {
        char new_path[PATH_MAX];
        snprintf(new_path, PATH_MAX, "%s/%s", path, filenames[i]);
        
        DIR *new_dir = opendir(new_path);
        if (new_dir) {
            printf("cd %s\n", filenames[i]);
            if (explore_directory(new_dir, new_path)) {
                return 1;
            }
            printf("cd ..\n");
        }
        free(filenames[i]);
    }
    free(filenames);
    return 0;
}


int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    DIR *dir = opendir(argv[1]);
    if (dir) {
        if (explore_directory(dir, argv[1])) {
            return 1;
        }
    }
    
    return 0;
}
