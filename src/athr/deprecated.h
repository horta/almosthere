#ifndef ATHR_DEPRECATED_H
#define ATHR_DEPRECATED_H

#include "athr/export.h"
#include "athr/option.h"

struct athr;

enum ATHR_OPTS
{
    ATHR_BAR_DEPRECATED = ATHR_BAR,
    ATHR_ETA_DEPRECATED = ATHR_ETA,
    ATHR_PERC_DEPRECATED = ATHR_PERC
};

typedef struct
{
    long volume;
    char const *desc;
    enum ATHR_OPTS opts;
} athr_create_args;

ATHR_API struct athr *athr_create_var(athr_create_args in);

#define athr_create(...) athr_create_var((athr_create_args){__VA_ARGS__});

ATHR_API void athr_consume(struct athr *at, long consume);
ATHR_API void athr_finish(struct athr *at);
ATHR_API void athr_sleep(long milliseconds);

#endif
