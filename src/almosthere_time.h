#ifndef ALMOSTHERE_TIME_H_
#define ALMOSTHERE_TIME_H_

#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#ifndef TIME_UTC
#define TIME_UTC 1
#endif
#endif

void almosthere_timespec_diff(struct timespec *start, struct timespec *stop,
                              struct timespec *diff);
double almosthere_timespec_sec(struct timespec *ts);
void almosthere_thread_sleep(double seconds);
int almosthere_timespec_get(struct timespec *ts);

#endif /* end of include guard: ALMOSTHERE_TIME_H_ */
