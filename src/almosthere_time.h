#ifndef ALMOSTHERE_TIME_H_
#define ALMOSTHERE_TIME_H_

#include <time.h>

// #ifdef WIN32
// #include <windows.h>
// #elif _POSIX_C_SOURCE >= 199309L
// #include <time.h>
// #else
// #include <unistd.h>
// #endif

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE < 199309L
#include <unistd.h>
#endif

/**
 * Cross-platform sleep function for C
 * @param int milliseconds
 */
void almosthere_timespec_diff(struct timespec *start, struct timespec *stop,
                              struct timespec *diff);
double almosthere_timespec_sec(struct timespec *ts);
void almosthere_thread_sleep(double seconds);
int almosthere_timespec_get(struct timespec *ts);

// #ifdef __unix__
// /* Yes it is a UNIX because __unix__ is defined.  */
//
// #include <unistd.h>
//
// /* You can find out the version with _POSIX_VERSION.
// ..
// ..  */
//
// #endif

#endif /* end of include guard: ALMOSTHERE_TIME_H_ */
