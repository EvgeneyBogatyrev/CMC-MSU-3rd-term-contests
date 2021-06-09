#include <stdio.h>
#include <stdlib.h>

enum
{ 
    FIRST_PRIME = 2,
    DIV_MIN = 2,
    MAX_N = 2000
};

int
is_prime(unsigned int number)
{
    if (number < FIRST_PRIME) {
        return 0;
    }

    for (unsigned int div = DIV_MIN; div * div <= number; ++div) {
        if (number % div == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(void)
{
    unsigned int N;
    if (scanf("%u", &N) != 1) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    if (N >= MAX_N) {
        fprintf(stderr, "The number is too large\n");
        return 1;
    }

    if (!is_prime(N)) {
        fprintf(stderr, "The number must be prime\n");
        return 1;
    }

    unsigned int *first_line = calloc(N - 1, sizeof(*first_line));
    unsigned int *prev_line = calloc(N - 1, sizeof(*prev_line));

    if (!first_line || !prev_line) {
        fprintf(stderr, "Memory limit exceeded\n");
        return 1;
    }

    for (unsigned int i = 0; i < N - 1; ++i) {
        printf("0 ");    
    }
    printf("\n");

    for (unsigned int number = 1; number < N; ++number) {
        for (unsigned int div = 1; div < N; ++div) {
            unsigned int result;
            if (number == 1) {
                unsigned int number_copy = number;
                while (number_copy % div != 0) {
                    number_copy += N;
                }
                result = number_copy / div;
                first_line[div - 1] = result;
            } else {
                result = (first_line[div - 1] + prev_line[div - 1]) % N;
            }
            prev_line[div - 1] = result;
            printf("%u ", result);    
        }
        printf("\n");
    }

    free(first_line);
    free(prev_line);
    return 0;
}
