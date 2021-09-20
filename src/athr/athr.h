#ifndef ATHR_ATHR_H
#define ATHR_ATHR_H

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(ATHR_DISABLE_DEPRECATED_API)
#include "athr/deprecated.h"
#endif

#include "athr/canvas.h"
#include "athr/canvas_view.h"
#include "athr/ema.h"
#include "athr/error.h"
#include "athr/export.h"
#include "athr/option.h"
#include "athr/rc.h"
#include "athr/thr.h"
#include "athr/widget/main.h"
#include "athr/widget/widget.h"
#include "elapsed/elapsed.h"
#include <stdatomic.h>
#include <stdbool.h>

struct athr
{
    unsigned long total;
    unsigned long consumed;
    struct athr_ema speed;
    struct elapsed elapsed;

    enum athr_option opts;
    struct athr_widget_main main;

    atomic_bool stop;
    atomic_flag lock;
    struct athr_thr thr;
    char error[ATHR_ERROR_SIZE];
};

#define ATHR_INIT                                                              \
    (struct athr)                                                              \
    {                                                                          \
        0, 0, ATHR_EMA_INIT, ELAPSED_INIT, ATHR_BAR, ATHR_WIDGET_MAIN_INIT,    \
            false, ATOMIC_FLAG_INIT, ATHR_THR_INIT,                            \
        {                                                                      \
            0                                                                  \
        }                                                                      \
    }

ATHR_API enum athr_rc athr_start(struct athr *at, unsigned long total,
                                 char const *desc, enum athr_option opts);
ATHR_API void athr_eat(struct athr *at, unsigned long amount);
ATHR_API void athr_stop(struct athr *at);

#ifdef __cplusplus
}
#endif

#endif
