#include <stdio.h>
#include <stdlib.h>

const long long int MULT = 1103515245;
const long long int INC = 12345;
const long long int MOD = 2147483648;


struct RandomGenerator;
struct RandomOperations;
void destroy(struct RandomGenerator *rr);
int next(struct RandomGenerator *rr);
struct RandomGenerator *random_create(int seed);


struct RandomOperations
{
    void (*destroy) (struct RandomGenerator *);
    int (*next) (struct RandomGenerator *);
};

struct RandomOperations ops;

typedef struct RandomGenerator
{
    long long int value;
    struct RandomOperations *ops;

} RandomGenerator;


void destroy(struct RandomGenerator *rr) 
{
    free(rr);
}

int next(struct RandomGenerator *rr)
{
    long long int res = rr->value * MULT;
    res %= MOD;
    res += INC;
    res %= MOD;

    rr->value = res;
    return (int) rr->value;
}

RandomGenerator *random_create(int seed)
{
    RandomGenerator *rr = calloc(1, sizeof(*rr));
    rr->value = seed;

    ops.destroy = destroy;
    ops.next = next;

    rr->ops = &ops;

    return rr;
}
