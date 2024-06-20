#ifndef ATHR_ATHR_H
#define ATHR_ATHR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "athr/canvas.h"
#include "athr/canvas_view.h"
#include "athr/ema.h"
#include "athr/export.h"
#include "athr/logger.h"
#include "athr/option.h"
#include "athr/ovs_atomic.h"
#include "athr/term.h"
#include "athr/thr.h"
#include "athr/widget.h"
#include "athr/widget_bar.h"
#include "athr/widget_eta.h"
#include "athr/widget_main.h"
#include "athr/widget_perc.h"
#include "athr/widget_text.h"

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

ATHR_API int athr_start(struct athr *at, uint64_t total, char const *desc,
                        enum athr_option opts);
ATHR_API void athr_eat(struct athr *at, uint64_t amount);
ATHR_API void athr_stop(struct athr *at);
ATHR_API void athr_disable_threading(bool disable);
ATHR_API int athr_sleep(unsigned milliseconds);

#ifdef __cplusplus
}
#endif

#endif
