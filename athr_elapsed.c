#include "athr_elapsed.h"

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#error "C11 or a more recent version is required"
#endif

#if defined(ATHR_OS_UNIX)
#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#endif

#include <time.h>

struct athr_elapsed {
    struct timespec start;
    struct timespec stop;
};

#ifndef TIME_UTC
#error Undefined TIME_UTC
#endif

#if defined(ATHR_OS_UNIX)
#include <sys/time.h>
#include <unistd.h>
#endif

#if defined(ATHR_OS_WIN32)
#include <windows.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct athr_elapsed* athr_elapsed_new(void)
{
    struct athr_elapsed* x = malloc(sizeof(struct athr_elapsed));
    return x;
}

void athr_elapsed_del(struct athr_elapsed* x)
{
    free(x);
}

static int get_any_timespec(struct timespec *ts)
{
    return timespec_get(ts, TIME_UTC) != TIME_UTC;
}

static void die(char const *msg)
{
    fprintf(stderr, "%s", msg);
    fflush(stderr);
    abort();
}

int athr_elapsed_start(struct athr_elapsed* x)
{
    return get_any_timespec(&x->start);
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

long athr_elapsed_milliseconds(struct athr_elapsed const* x)
{
    struct timespec diff = { 0 };
    timespecsub(&x->stop, &x->start, &diff);
    if (diff.tv_sec < 0 || diff.tv_nsec < 0)
        die("unexpected negative time duration");

    long ms = (long)(diff.tv_sec * MILLISECONDS_IN_A_SECOND);
    long re = (long)((diff.tv_nsec % NANOSECONDS_IN_A_SECOND) / MICROSECONDS_IN_A_SECOND);
    return ms + re;
}

int athr_elapsed_stop(struct athr_elapsed* x)
{
    return get_any_timespec(&x->stop);
}

int athr_elapsed_sleep(long ms)
{
    struct timespec duration = { .tv_sec = (time_t)(ms / MILLISECONDS_IN_A_SECOND),
        .tv_nsec = (long)((ms % MILLISECONDS_IN_A_SECOND) * 1000000) };

#if defined(ATHR_OS_UNIX)
    return nanosleep(&duration, NULL);
#endif

#if defined(ATHR_OS_WIN32)
    struct timespec start;

    timespec_get(&start, TIME_UTC);
    time_t milliseconds = duration.tv_sec * MILLISECONDS_IN_A_SECOND;
    milliseconds += duration.tv_nsec / MICROSECONDS_IN_A_SECOND;
    milliseconds += (((duration.tv_nsec % MICROSECONDS_IN_A_SECOND) == 0) ? 0 : 1);

    return (int)SleepEx((DWORD)milliseconds, TRUE);
#endif
}
