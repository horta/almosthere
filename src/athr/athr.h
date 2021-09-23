#ifndef ATHR_ATHR_H
#define ATHR_ATHR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "athr/canvas.h"
#include "athr/canvas_view.h"
#include "athr/deprecated.h"
#include "athr/ema.h"
#include "athr/export.h"
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
#include "elapsed/elapsed.h"

struct athr
{
    unsigned long total;
    atomic_ulong consumed;
    struct athr_ema speed;
    struct elapsed elapsed;

    enum athr_option opts;
    struct athr_widget_main main;

    atomic_bool stop;
    atomic_flag lock;
    struct athr_thr thr;
    char const *error;
};

#define ATHR_INIT                                                              \
    (struct athr)                                                              \
    {                                                                          \
        0, 0, ATHR_EMA_INIT, ELAPSED_INIT, ATHR_BAR, ATHR_WIDGET_MAIN_INIT,    \
            false, ATOMIC_FLAG_INIT, ATHR_THR_INIT, NULL                       \
    }

ATHR_API int athr_start(struct athr *at, unsigned long total, char const *desc,
                        enum athr_option opts);
ATHR_API void athr_eat(struct athr *at, unsigned long amount);
ATHR_API void athr_stop(struct athr *at);
ATHR_API void athr_disable_threading(bool disable);
ATHR_API int athr_sleep(unsigned milliseconds);
static inline char const *athr_error(struct athr const *at)
{
    return at->error;
}
static inline void athr_clear_error(struct athr *at) { at->error = NULL; }

#ifdef __cplusplus
}
#endif

#endif
