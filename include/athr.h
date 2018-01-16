#ifndef ATHR_H_
#define ATHR_H_

struct athr;

enum ATHR_OPTS { ATHR_BAR = 1, ATHR_ETA = 2, ATHR_PERC = 4 };

struct athr *athr_create(long volume, const char *, enum ATHR_OPTS);
void athr_consume(struct athr *at, long consume);
void athr_finish(struct athr *at);
void athr_sleep(long milliseconds);

#endif /* end of include guard: ATHR_H_ */
