#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

enum
{
    DIR_UP_LEN = 3,
    CUR_DIR_LEN = 2,
    INIT_SIZE = 4
};

char *relativize_path(const char *path1, const char *path2);
char *normalize(const char *path);


char *
normalize(const char *path)
{
    char *result = calloc(strlen(path) + 1, 1);
    int counter = strlen(path);
    result[counter--] = '\0';

    char *end = (char *) path + strlen(path);
    char *start = end - 1;

    int dir_len = 0;
    while (1) {
        while (start > path && *start != '/') {
            start--;
        }
        
        if (start < path) {
            break;
        }
        if (!strncmp(start, "/.", CUR_DIR_LEN) 
                && (*(start + CUR_DIR_LEN) == '/' || !*(start + CUR_DIR_LEN))) {
            ;
        } else if (!strncmp(start, "/..", DIR_UP_LEN)
                && (*(start + DIR_UP_LEN) == '/' || !*(start + DIR_UP_LEN))) {
            dir_len++;
        } else {
            if (dir_len > 0) {
                dir_len--;
            } else {
                for (char *p = end - 1; p >= start; --p) {
                    result[counter--] = *p;
                }
            }
        }
        end = start;
        start--;
    }

    result = memmove(result, result + counter + 1, strlen(result + counter + 1));
    return result;
}


char *
relativize_path(const char *path1, const char *path2)
{
    char *path_from = normalize(path1);
    char *path_to = normalize(path2);

    int index;
    int diff_index = 0;
    for (index = 0; path_from[index] && path_from[index] == path_to[index]; ++index) {
        if (path_from[index] == '/') {
            diff_index = index;
        }
    }

    if (path_to[index] == '\0' && path_from[index] == '/') {
        diff_index = index;
    }

    int size = 0;
    int max_size = INIT_SIZE;
    char *result = calloc(max_size, 1);

    int dir_len = 0;
    for (int i = diff_index + 1; path_from[i]; ++i) {
        if (path_from[i] == '/') {

            size += DIR_UP_LEN;
            if (size >= max_size) {
                max_size += max_size;
                result = realloc(result, max_size);
            }
            result = strcat(result, "../");
            dir_len++;
        }
    }

    if (path_to[diff_index] && path_to[diff_index + 1]) {
        int additional = strlen(path_to + diff_index + 1);
        if (max_size - size < additional) {
            max_size += additional;
            result = realloc(result, max_size);
        }
        strcat(result, path_to + diff_index + 1);
        strcat(result, "\0");

    } else if (dir_len == 0) {
        result[0] = '.';
        result[1] = '\0';
    } else {
        if (strlen(result) != 0) {
            result[strlen(result) - 1] = '\0';
        }
    }
    
    free(path_from);
    free(path_to);
    return result;

}

char *
relativize_path_old(const char *path1, const char *path2)
{
    char *path_from = (char *) path1;//normalize(path1);
    char *path_to = (char *) path2; //normalize(path2);
    char *result = calloc(2 * PATH_MAX, 1);

    char *endptr = path_to + strlen(path_to);
    while (strncmp(path_from, path_to, endptr - path_to)) {
        do {
            endptr--;
        } while (endptr != path_to && *endptr != '/');
    }
    int prefix_len = endptr - path_to;

    char *ending = path_from + prefix_len;
    
    int dir_len = 0;
    if (*ending == '/') {
        ending++;
    }
    while (*ending) {
        if (*ending == '/') {
            dir_len++;
        }
        ++ending;
    }
    
    char *cur = result;
    for (int i = 0; i < dir_len; ++i) {
        if (i == dir_len - 1) {
            strcpy(cur, "..");
            cur += DIR_UP_LEN - 1;
        } else {
            strcpy(cur, "../");
            cur += DIR_UP_LEN;
        }
    }
    if (dir_len == 0 && *(path_to + prefix_len) == '/') {
        prefix_len++;
    }
    strcpy(cur, path_to + prefix_len);

    if (strlen(result) == 0) {
        result[0] = '.';
        result[1] = '\0';
    }

    //free(path_from);
    //free(path_to);
    return result;
}
