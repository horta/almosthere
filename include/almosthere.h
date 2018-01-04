#ifndef ALMOSTHERE_H_
#define ALMOSTHERE_H_

struct almosthere;

struct almosthere *almosthere_create(long volume);
void almosthere_consume(struct almosthere *at, long consume);
void almosthere_finish(struct almosthere *at);
void almosthere_sleep(long milliseconds);

#endif /* end of include guard: ALMOSTHERE_H_ */
