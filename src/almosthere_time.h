#ifndef ALMOSTHERE_TIME_H_
#define ALMOSTHERE_TIME_H_

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#endif

/**
 * Cross-platform sleep function for C
 * @param int milliseconds
 */
void sleep_ms(int milliseconds);
void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result);
double timespec_seconds(struct timespec *ts);

#endif /* end of include guard: ALMOSTHERE_TIME_H_ */
