#include "athr_time.h"
#include "athr.h"
#include "thread/thread.h"

#ifdef __APPLE__
#ifndef TIME_UTC
#define TIME_UTC 1
#endif
#elif __linux__
#include <unistd.h>
#define POSIX_SYSTEM
#endif

static void sec_timespec(double seconds, struct timespec* ts);

double athr_timespec_sec(struct timespec* ts)
{
    return ts->tv_sec + ts->tv_nsec / 1000.0 / 1000.0 / 1000.0;
}

int athr_timespec_get(struct timespec* ts)
{
#ifdef POSIX_SYSTEM
    return clock_gettime(CLOCK_REALTIME, ts);
#else
    return timespec_get(ts, TIME_UTC);
#endif
}

void athr_thread_sleep(double seconds)
{

    struct timespec ts;
    sec_timespec(seconds, &ts);
    thrd_sleep(&ts, NULL);
}

void athr_timespec_diff(struct timespec* start, struct timespec* stop, struct timespec* diff)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        diff->tv_sec = stop->tv_sec - start->tv_sec - 1;
        diff->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        diff->tv_sec = stop->tv_sec - start->tv_sec;
        diff->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }

    return;
}

static void sec_timespec(double seconds, struct timespec* ts)
{

    int    isec = (int)seconds;
    double remainder;
    ts->tv_sec = isec;
    remainder = seconds - ((double)isec);
    ts->tv_nsec = (long)(remainder * 1000 * 1000 * 1000);
}
