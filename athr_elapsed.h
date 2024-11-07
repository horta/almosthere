#ifndef ATHR_ELAPSED_H
#define ATHR_ELAPSED_H

struct athr_elapsed;

struct athr_elapsed *athr_elapsed_new(void);
void                 athr_elapsed_del(struct athr_elapsed *);
int                  athr_elapsed_start(struct athr_elapsed *);
int                  athr_elapsed_stop(struct athr_elapsed *);
long                 athr_elapsed_milliseconds(struct athr_elapsed const *);
int                  athr_elapsed_sleep(long milliseconds);

#endif
