#ifndef EMA_H
#define EMA_H

#include "athr/ema.h"
#include "common.h"
#include <assert.h>

#define SMOOTHING 0.3f

/* 1 / (1 - pow(1 - SMOOTHING, n)) for n == 1, 2, ... */
static double const factor[15] = {
    3.33f, 1.96f, 1.52f, 1.32f, 1.20f, 1.13f, 1.09f, 1.06f,
    1.04f, 1.03f, 1.02f, 1.01f, 1.01f, 1.01f, 1.00f,
};

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
