#ifndef ALMOSTHERE_H_
#define ALMOSTHERE_H_

/* Minimum different in seconds considered safe for computing speed. */
static const double ALMOSTHERE_MIN_DLT = 0.5;

/* how often to update, in seconds */
static const double ALMOSTHERE_TIMESTEP = 1.0 / 30.0;

struct almosthere;

struct almosthere *almosthere_create(long volume);
void almosthere_consume(struct almosthere *at, long consume);
void almosthere_finish(struct almosthere *at);
void almosthere_sleep(long milliseconds);

#endif /* end of include guard: ALMOSTHERE_H_ */
