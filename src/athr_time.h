#ifndef ATHR_TIME_H
#define ATHR_TIME_H

#include "hide.h"
#include <time.h>

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

HIDE double athr_timespec_sec(struct timespec* ts);
HIDE int    athr_timespec_get(struct timespec* ts);
HIDE void   athr_thread_sleep(double seconds);
HIDE void   athr_timespec_diff(struct timespec* start, struct timespec* stop,
                               struct timespec* diff);

#endif
