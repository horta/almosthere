#ifndef ATHR_EMA_H
#define ATHR_EMA_H

/* Exponential moving average */
struct athr_ema
{
    double last;
    unsigned calls;
};

#define ATHR_EMA_INIT                                                          \
    (struct athr_ema) { 0., 0 }

#endif
