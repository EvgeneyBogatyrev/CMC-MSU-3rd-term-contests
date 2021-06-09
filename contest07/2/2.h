struct RandomGenerator;
void destroy(struct RandomGenerator *rr);
int next(struct RandomGenerator *rr);
struct RandomGenerator *random_create(int seed);

struct RandomOperations
{
    void (*destroy) (struct RandomGenerator *);
    int (*next) (struct RandomGenerator *);
};
