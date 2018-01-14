#ifndef ATHR_TIME_H_
#define ATHR_TIME_H_

#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "thread/thread.h"

void athr_timespec_diff(struct timespec *start, struct timespec *stop,
                        struct timespec *diff);
double athr_timespec_sec(struct timespec *ts);
void athr_thread_sleep(double seconds);
int athr_timespec_get(struct timespec *ts);

#endif /* end of include guard: ATHR_TIME_H_ */
