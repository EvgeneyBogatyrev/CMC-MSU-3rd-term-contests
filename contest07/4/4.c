#include <dlfcn.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    ARG_LEN = 8,
    FNAME_INDEX = 2,
    SIG_INDEX = 3,
    ARGS_INDEX = 4
};

int
main(int argc, char *argv[])
{
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Cannot open\n");
        return 1;
    }

    uint64_t arg[ARG_LEN] = {0};
    void *cur_point = (void *) &arg;

    int int_data;
    double double_data;

    for (int i = ARGS_INDEX; i < argc; ++i) {
        switch (argv[SIG_INDEX][i - ARGS_INDEX + 1]) {
        case 'i':
            sscanf(argv[i], "%d", &int_data);
            memmove(cur_point, &int_data, sizeof(int_data));
            cur_point += sizeof(int_data);
            break;
        case 'd':
            sscanf(argv[i], "%lf", &double_data);
            memmove(cur_point, &double_data, sizeof(double_data));
            cur_point += sizeof(double_data);
            if (sizeof(double_data) % 4 != 0) {
                cur_point++;
            }
            break;
        case 's':
            memmove(cur_point, &argv[i], sizeof(char *));
            cur_point += sizeof(char *);
            break;
        default:
            fprintf(stderr, "Wrong data type\n");
            return 1;
        }
    }
   
    if (argv[SIG_INDEX][0] == 'v') {
        void (*function) (uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t, uint64_t, uint64_t) = dlsym(handle, argv[FNAME_INDEX]);
       
        function(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]);

    } else if (argv[SIG_INDEX][0] == 'i') {
        int (*function) (uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t, uint64_t, uint64_t) = dlsym(handle, argv[FNAME_INDEX]);
       
        int result = function(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]);
        printf("%d\n", result);    

    } else if (argv[SIG_INDEX][0] == 'd') {
        double (*function) (uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t, uint64_t, uint64_t) = dlsym(handle, argv[FNAME_INDEX]);
       
        double result = function(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]);
        printf("%.10g\n", result);    

    } else if (argv[SIG_INDEX][0] == 's') {
        char * (*function) (uint64_t, uint64_t, uint64_t, uint64_t, 
                uint64_t, uint64_t, uint64_t, uint64_t) = dlsym(handle, argv[FNAME_INDEX]);
       
        char *result = function(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]);
        printf("%s\n", result);   

    } else {
        fprintf(stderr, "Wrong return type\n");
        return 1;
    }

    dlclose(handle);
    return 0;
}
