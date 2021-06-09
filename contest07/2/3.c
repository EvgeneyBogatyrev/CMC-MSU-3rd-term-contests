#include <stdio.h>
#include <stdlib.h>

enum
{
    MULT = 1103515245,
    INC = 12345,
    MOD = 2147483648
};


struct RandomGenerator;
void destroy(struct RandomGenerator *rr);
int next(struct RandomGenerator *rr);
struct RandomGenerator *random_create(int seed);

struct RandomOperations
{
    void (*destroy) (struct RandomGenerator *);
    int (*next) (struct RandomGenerator *);
} ops;

typedef struct RandomGenerator
{
    int value;
    struct RandomOperations *ops;

} RandomGenerator;


void destroy(struct RandomGenerator *rr) 
{
    free(rr);
}

int next(struct RandomGenerator *rr)
{
    long long res = (long long) rr->value * MULT;
    res %= MOD;
    res += INC;
    res %= MOD;

    rr->value = (int) res;
    return rr->value;
}

RandomGenerator *random_create(int seed)
{
    RandomGenerator *rr = calloc(1, sizeof(rr));
    rr->value = seed;

    ops.destroy = &destroy;
    ops.next = &next;

    rr->ops = &ops;

    return rr;
}
