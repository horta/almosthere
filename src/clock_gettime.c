#include "clock_gettime.h"
#include <stddef.h>
#include <sys/time.h>
#include <stdio.h>

// clock_gettime is not implemented on older versions of OS X (< 10.12).
int athr_clock_gettime(struct timespec* t)
{
    struct timeval now;
    int            rv = gettimeofday(&now, NULL);
    if (rv)
        return rv;
    t->tv_sec = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
