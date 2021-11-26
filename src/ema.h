#ifndef EMA_H
#define EMA_H

#include "athr/ema.h"
#include "common.h"
#include <assert.h>

#define SMOOTHING 0.2f

/* 1 / (1 - pow(1 - SMOOTHING, n)) for n == 1, 2, ... */
static double const factor[] = {2.78, 2.05, 1.69, 1.49, 1.36, 1.27, 1.20, 1.16,
                                1.12, 1.09, 1.07, 1.06, 1.05, 1.04, 1.03, 1.02,
                                1.02, 1.01, 1.01, 1.01, 1.01, 1.01, 1.00};

static inline void ema_add(struct athr_ema *ema, double x)
{
    ema->last = SMOOTHING * x + (1 - SMOOTHING) * ema->last;
    ema->calls = minu(ema->calls + 1, ARRAY_SIZE(factor));
}

static inline double ema_get(struct athr_ema const *ema)
{
    assert(ema->calls > 0);
    return ema->last * factor[(unsigned)(ema->calls - 1)];
}

#endif
