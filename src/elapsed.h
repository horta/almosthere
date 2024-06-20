#ifndef ELAPSED_H
#define ELAPSED_H

struct elapsed;

struct elapsed* elapsed_new(void);
void elapsed_del(struct elapsed*);

int elapsed_start(struct elapsed*);
int elapsed_stop(struct elapsed*);

long elapsed_milliseconds(struct elapsed const*);

int elapsed_sleep(long milliseconds);

#endif
