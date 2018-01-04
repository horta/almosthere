#ifndef ALMOSTHERE_TIME_H_
#define ALMOSTHERE_TIME_H_

#include "posix_check.h"
#include <time.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef POSIX_SYSTEM
#include <unistd.h>
#endif

void almosthere_timespec_diff(struct timespec *start, struct timespec *stop,
                              struct timespec *diff);
double almosthere_timespec_sec(struct timespec *ts);
void almosthere_thread_sleep(double seconds);
int almosthere_timespec_get(struct timespec *ts);

#endif /* end of include guard: ALMOSTHERE_TIME_H_ */
