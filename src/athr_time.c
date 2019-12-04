#include "athr_time.h"
#include "athr.h"
#include "clock_gettime.h"
#include "thread/thread.h"

static void sec_timespec(double seconds, struct timespec* ts);

double athr_timespec_sec(struct timespec* ts)
{
    double f = ((double)ts->tv_nsec) / 1000.0 / 1000.0 / 1000.0;
    return ((double)ts->tv_sec) + f;
}

int athr_timespec_get(struct timespec* ts)
{
#if defined(HAVE_TIMESPEC_GET)
    return timespec_get(ts, 1);
#elif defined(HAVE_CLOCK_GETTIME)
    return clock_gettime(ATHR_CLOCK_TYPE, ts);
#else
    return athr_clock_gettime(ts);
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
