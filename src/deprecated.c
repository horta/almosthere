#include "athr/deprecated.h"
#include "athr/athr.h"

struct athr *athr_create_var(athr_create_args in)
{
    long volume_out = in.volume ? in.volume : 0;
    const char *desc_out = in.desc ? in.desc : NULL;
    enum ATHR_OPTS opts_out =
        in.opts
            ? in.opts
            : ATHR_BAR_DEPRECATED | ATHR_ETA_DEPRECATED | ATHR_PERC_DEPRECATED;

    struct athr *at = malloc(sizeof *at);
    *at = ATHR_INIT;
    athr_start(at, volume_out, desc_out, (enum athr_option)opts_out);
    return at;
}

void athr_consume(struct athr *at, long consume) { athr_eat(at, consume); }

void athr_finish(struct athr *at) { athr_stop(at); }

void athr_sleep(long milliseconds) { elapsed_sleep(milliseconds / 1000.); }
