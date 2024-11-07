#include "athr_elapsed.h"

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#error "C11 or a more recent version is required"
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
#define WINDOWS_OS
#else
#define POSIX_OS
#endif

#if defined(POSIX_OS)
#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#endif

#include <time.h>

struct elapsed {
    struct timespec start;
    struct timespec stop;
};

#ifndef TIME_UTC
#error Undefined TIME_UTC
#endif

#if defined(POSIX_OS)
#include <sys/time.h>
#include <unistd.h>
#endif

#if defined(WINDOWS_OS)
#include <windows.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct elapsed* elapsed_new(void)
{
    struct elapsed* x = malloc(sizeof(struct elapsed));
    return x;
}

void elapsed_del(struct elapsed* elapsed)
{
    free(elapsed);
}

static int
get_any_timespec(struct timespec* ts)
{
    return timespec_get(ts, TIME_UTC) != TIME_UTC;
}

static void
die(char const* msg)
{
    fprintf(stderr, "%s", msg);
    fflush(stderr);
    abort();
}

int elapsed_start(struct elapsed* elapsed)
{
    return get_any_timespec(&elapsed->start);
}

/* source: libbsd */
#ifndef timespecsub
#define timespecsub(tsp, usp, vsp)                        \
    do {                                                  \
        (vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;    \
        (vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec; \
        if ((vsp)->tv_nsec < 0) {                         \
            (vsp)->tv_sec--;                              \
            (vsp)->tv_nsec += 1000000000L;                \
        }                                                 \
    } while (0)
#endif

#define MILLISECONDS_IN_A_SECOND 1000L
#define MICROSECONDS_IN_A_SECOND (MILLISECONDS_IN_A_SECOND * 1000L)
#define NANOSECONDS_IN_A_SECOND (MICROSECONDS_IN_A_SECOND * 1000L)

long elapsed_milliseconds(struct elapsed const* elapsed)
{
    struct timespec diff = { 0 };
    timespecsub(&elapsed->stop, &elapsed->start, &diff);
    if (diff.tv_sec < 0 || diff.tv_nsec < 0)
        die("unexpected negative time duration");

    long ms = (long)(diff.tv_sec * MILLISECONDS_IN_A_SECOND);
    long re = (long)((diff.tv_nsec % NANOSECONDS_IN_A_SECOND) / MICROSECONDS_IN_A_SECOND);
    return ms + re;
}

int elapsed_stop(struct elapsed* elapsed)
{
    return get_any_timespec(&elapsed->stop);
}

int elapsed_sleep(long ms)
{
    struct timespec duration = { .tv_sec = (time_t)(ms / MILLISECONDS_IN_A_SECOND),
        .tv_nsec = (long)((ms % MILLISECONDS_IN_A_SECOND) * 1000000) };

#if defined(POSIX_OS)
    return nanosleep(&duration, NULL);
#endif

#if defined(WINDOWS_OS)
    struct timespec start;

    timespec_get(&start, TIME_UTC);
    time_t milliseconds = duration.tv_sec * MILLISECONDS_IN_A_SECOND;
    milliseconds += duration.tv_nsec / MICROSECONDS_IN_A_SECOND;
    milliseconds += (((duration.tv_nsec % MICROSECONDS_IN_A_SECOND) == 0) ? 0 : 1);

    return (int)SleepEx((DWORD)milliseconds, TRUE);
#endif
}
