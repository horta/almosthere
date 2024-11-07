#ifndef ATHR_H
#define ATHR_H

#include "athr_ema.h"
#include "athr_os.h"
#include "athr_option.h"
#include "athr_thr.h"
#include "athr_widget_main.h"
#include <stdint.h>

#if defined(ATHR_WINDOWS)
#include "athr_ovs_atomic_msvc.h"
#elif defined(ATHR_POSIX)
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#endif

/* How often to update initially, in milliseconds. */
#define ATHR_TIMESTEP 250

/* Limit on how long to update, in milliseconds. */
#define ATHR_TIMESTEP_LIMIT 750

struct elapsed;

struct athr
{
    unsigned timestep;
    uint64_t total;
    atomic_uint_fast64_t consumed;
    uint_fast64_t last_consumed;
    struct athr_ema speed;
    struct elapsed *elapsed;
    struct elapsed *total_elapsed;

    enum athr_option opts;
    struct athr_widget_main main;

    atomic_bool stop;
    atomic_flag lock;
    struct athr_thr thr;
};

#define ATHR_INIT                                                              \
    (struct athr)                                                              \
    {                                                                          \
        ATHR_TIMESTEP, 0, 0, 0, ATHR_EMA_INIT, NULL, NULL, ATHR_BAR,           \
            ATHR_WIDGET_MAIN_INIT, false, ATOMIC_FLAG_INIT, ATHR_THR_INIT      \
    }

int  athr_start(struct athr *at, uint64_t total, char const *desc,
                enum athr_option opts);
void athr_eat(struct athr *at, uint64_t amount);
void athr_stop(struct athr *at);
void athr_disable_threading(bool disable);
int  athr_sleep(unsigned milliseconds);

// PRIVATE ------------------------------------------------------------------
struct athr;

void athr_stop_wait(struct athr *at);
// PRIVATE ------------------------------------------------------------------

#endif
