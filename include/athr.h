#ifndef ATHR_H
#define ATHR_H

#include "athr_export.h"

#ifdef __cplusplus
extern "C"
{
#endif

    struct athr;

    enum ATHR_OPTS
    {
        ATHR_BAR = 1,
        ATHR_ETA = 2,
        ATHR_PERC = 4
    };

    typedef struct
    {
        long           volume;
        char const*    desc;
        enum ATHR_OPTS opts;
    } athr_create_args;

    ATHR_EXPORT struct athr* athr_create_var(athr_create_args in);

#define athr_create(...) athr_create_var((athr_create_args){__VA_ARGS__});

    ATHR_EXPORT void athr_consume(struct athr* at, long consume);
    ATHR_EXPORT void athr_finish(struct athr* at);
    ATHR_EXPORT void athr_sleep(long milliseconds);

#ifdef __cplusplus
}
#endif

#endif
