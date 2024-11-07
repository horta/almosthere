#ifndef ATHR_EMA_H
#define ATHR_EMA_H

#include "athr_array.h"
#include "athr_min.h"
#include <assert.h>

/* Exponential moving average */
struct athr_ema
{
    double   last;
    unsigned calls;
};

#define ATHR_EMA_INIT                                                          \
    (struct athr_ema) { 0., 0 }

#define ATHR_SMOOTHING 0.2f

/* 1 / (1 - pow(1 - ATHR_SMOOTHING, n)) for n == 1, 2, ... */
static double const factor[] = {2.78, 2.05, 1.69, 1.49, 1.36, 1.27, 1.20, 1.16,
                                1.12, 1.09, 1.07, 1.06, 1.05, 1.04, 1.03, 1.02,
                                1.02, 1.01, 1.01, 1.01, 1.01, 1.01, 1.00};

static inline void athr_ema_add(struct athr_ema *ema, double x)
{
    ema->last = ATHR_SMOOTHING * x + (1 - ATHR_SMOOTHING) * ema->last;
    ema->calls = athr_min(ema->calls + 1, athr_array_size(factor));
}

static inline double athr_ema_get(struct athr_ema const *ema)
{
    assert(ema->calls > 0);
    return ema->last * factor[(unsigned)(ema->calls - 1)];
}

#endif
