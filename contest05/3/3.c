#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum 
{ 
    DIR_UP_LEN = 3,
    CUR_DIR_LEN = 2
};

char *relativize_path(const char *path1, const char *path2);
static char *get_next(char *startptr, char *endptr);
static char *form_answer(char *end1, char *end2);
static char *normalize(const char *path);

static char *
normalize(const char *path)
{
    int len = strlen(path);
    char *new_path = calloc(len, 1);
    strcpy(new_path, path);
    
    for (char *p = new_path + len - 1; p >= new_path; --p) {
        if (!strncmp(p, "/.", CUR_DIR_LEN) 
                && (*(p + CUR_DIR_LEN) == '/' || *(p + CUR_DIR_LEN) == '\0')) {
            strcpy(p, p + CUR_DIR_LEN);
        }
    }

    int dir_count = 0;
    for (char *p = new_path + len - 1; p >= new_path; --p) {
        if (!strncmp(p, "/..", DIR_UP_LEN) 
                && (*(p + DIR_UP_LEN) == '/' || *(p + DIR_UP_LEN) == '\0')) {
            strcpy(p, p + DIR_UP_LEN);
            dir_count++;
        } else if (*p == '/' && dir_count > 0) {
            char *forward_p = p + 1;
            while (*forward_p != '/' && *forward_p != '\0') {
                ++forward_p;
            }
            strcpy(p, p + (forward_p - p));
           dir_count--; 
        }
    }

    if (strlen(new_path) == 0) {
        new_path[0] = '/';
        new_path[1] = '\0';
    }

    return new_path;
}

static char *
form_answer(char *end1, char *end2)
{
    int dir_count = 0;
    while (*end1 && (end1 = strstr(end1 + 1, "/"))) {
        dir_count++;
    }   

    int total_length = DIR_UP_LEN * dir_count + strlen(end2);
    char *answer = calloc(total_length, 1);
    char *ptr = answer;
    
    for (int i = 0; i < dir_count; ++i) {
        if (i < dir_count - 1) {
            strcpy(ptr, "../");
            ptr += DIR_UP_LEN;
        } else {
            strcpy(ptr, "..");
            ptr += DIR_UP_LEN - 1;
        }
    }

    strcpy(ptr, end2);
    if (strlen(answer) == 0) {
        answer[0] = '.';
        answer[1] = '\0';
    }
    return answer;
}

static char *
get_next(char *startptr, char *endptr)
{
    char *p = endptr;
    while (*p != '/' && p != startptr) {
        --p;
    }

    return p;
}

char *
relativize_path(const char *path1, const char *path2)
{
    char *path_from = normalize(path1);
    char *path_to = normalize(path2);

    char *endptr = path_to + strlen(path_to);

    while (1) {
        char *startptr = get_next(path_to, endptr - 1);

        if (!strncmp(path_from, path_to, endptr - path_to)) {
            char *answer = form_answer(path_from + (endptr - path_to), endptr);
            free(path_from);
            free(path_to);
            return answer;
        }
        
        if (startptr == path_to) {
            break;
        }
        endptr = startptr;
    }

    char *answer = form_answer(path_from, path_to);
    free(path_from);
    free(path_to);
    return answer;
}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        return 1;
    }

    printf("%s\n", relativize_path(argv[1], argv[2]));
    return 0;
}
