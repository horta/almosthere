#ifndef CLOCK_GETTIME_H
#define CLOCK_GETTIME_H

#include "hide.h"
#include <time.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* https://stackoverflow.com/a/50245503 */
/* We prefer CLOCK_MONOTONIC but POSIX does not enforce the implementation of it. */
#if !defined(_POSIX_MONOTONIC_CLOCK) || _POSIX_MONOTONIC_CLOCK < 0
#define ATHR_CLOCK_TYPE CLOCK_REALTIME
#elif _POSIX_MONOTONIC_CLOCK > 0
#define ATHR_CLOCK_TYPE CLOCK_MONOTONIC
#elif defined(CLOCK_REALTIME)
#define ATHR_CLOCK_TYPE CLOCK_REALTIME
#else
#define ATHR_CLOCK_TYPE 0
#endif

struct timespec;

HIDE int athr_clock_gettime(struct timespec* t);

#endif
