#ifndef ATHR_ELAPSED_H
#define ATHR_ELAPSED_H

// PRIVATE ------------------------------------------------------------------
struct elapsed;

struct elapsed* elapsed_new(void);
void elapsed_del(struct elapsed*);

int elapsed_start(struct elapsed*);
int elapsed_stop(struct elapsed*);

long elapsed_milliseconds(struct elapsed const*);

int elapsed_sleep(long milliseconds);
// PRIVATE ------------------------------------------------------------------

#endif
