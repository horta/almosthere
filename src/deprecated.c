#include "athr/deprecated.h"
#include "athr.h"
#include "athr/athr.h"
#include "logger.h"

struct athr *athr_create_var(athr_create_args in)
{
    athr_canvas_use_stderr(true);
    long volume_out = in.volume ? in.volume : 0;
    const char *desc_out = in.desc ? in.desc : NULL;
    enum ATHR_OPTS opts_out =
        in.opts
            ? in.opts
            : ATHR_BAR_DEPRECATED | ATHR_ETA_DEPRECATED | ATHR_PERC_DEPRECATED;

    struct athr *at = malloc(sizeof *at);
    if (!at)
    {
        error("failed to malloc");
        return NULL;
    }
    *at = ATHR_INIT;
    athr_start(at, (unsigned long)volume_out, desc_out,
               (enum athr_option)opts_out);
    return at;
}

void athr_consume(struct athr *at, long consume)
{
    athr_eat(at, (unsigned long)consume);
}

void athr_finish(struct athr *at)
{
    athr_stop_wait(at);
    free(at);
}
