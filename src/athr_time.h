#ifndef ATHR_TIME_H
#define ATHR_TIME_H

#include <time.h>

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

double athr_timespec_sec(struct timespec* ts);
int    athr_timespec_get(struct timespec* ts);
void   athr_thread_sleep(double seconds);
void athr_timespec_diff(struct timespec* start, struct timespec* stop, struct timespec* diff);

#endif
