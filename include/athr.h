#ifndef ATHR_H_
#define ATHR_H_

struct athr;

enum ATHR_OPTS { ATHR_BAR = 1, ATHR_ETA = 2, ATHR_PERC = 4 };

typedef struct {
    long volume;
    const char *desc;
    enum ATHR_OPTS opts;
} athr_create_args;

struct athr *athr_create_var(athr_create_args in);
#define athr_create(...) athr_create_var((athr_create_args){__VA_ARGS__});

void athr_consume(struct athr *at, long consume);
void athr_finish(struct athr *at);
void athr_sleep(long milliseconds);

#endif /* end of include guard: ATHR_H_ */
