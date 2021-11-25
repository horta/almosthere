#include "athr/athr.h"
#include "athr.h"
#include "athr/widget_main.h"
#include "ema.h"
#include "error.h"
#include "thr.h"
#include "widget_bar.h"
#include "widget_eta.h"
#include "widget_main.h"
#include "widget_perc.h"
#include "widget_text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static atomic_bool disable_thread = false;

static void lock(struct athr *at)
{
    while (atomic_flag_test_and_set(&at->lock))
        /* spin until the lock is acquired */;
}

static void unlock(struct athr *at) { atomic_flag_clear(&at->lock); }

static void update(struct athr *at)
{
    lock(at);
    uint_fast64_t consumed = atomic_load_uint_fast64(&at->consumed);
    if (consumed == at->last_consumed) goto cleanup;
    uint_fast64_t delta = consumed - at->last_consumed;
    at->last_consumed = consumed;

    if (elapsed_stop(&at->elapsed)) error(at, "failed to elapsed_stop");

    double seconds = ((double)elapsed_milliseconds(&at->elapsed)) / 1000.;
    double progress = ((double)delta) / ((double)at->total);

    if (progress < 0.001f && at->timestep < ATHR_TIMESTEP_LIMIT)
    {
        at->timestep += ATHR_TIMESTEP;
        if (at->timestep > ATHR_TIMESTEP_LIMIT)
            at->timestep = ATHR_TIMESTEP_LIMIT;
        ema_reset(&at->speed);
    }

    ema_add(&at->speed, progress / seconds);

    double consumed_fraction = ((double)consumed) / ((double)at->total);
    at->main.super.vtable->update(&at->main.super, consumed_fraction,
                                  ema_get(&at->speed));

    if (elapsed_start(&at->elapsed)) error(at, "failed to elapsed_start");
cleanup:
    unlock(at);
}

static void thread_start(void *args)
{
    struct athr *at = (struct athr *)args;
    while (!atomic_load_bool(&at->stop) && !atomic_load_bool(&disable_thread))
    {
        update(at);
        if (athr_sleep(at->timestep)) error(at, "failed to sleep");
    }
}

int athr_start(struct athr *at, uint64_t total, const char *desc,
               enum athr_option opts)
{
    if (desc == NULL) desc = "";
    at->timestep = ATHR_TIMESTEP;
    at->total = total;
    at->consumed = 0;
    at->last_consumed = 0;
    at->speed = ATHR_EMA_INIT;
    if (elapsed_start(&at->elapsed))
    {
        error(at, "failed to elapsed_start");
        return 1;
    }

    at->opts = opts;
    widget_main_create(&at->main);
    widget_text_create(widget_main_add_text(&at->main), desc);
    if (opts & ATHR_PERC) widget_perc_create(widget_main_add_perc(&at->main));
    if (opts & ATHR_BAR) widget_bar_create(widget_main_add_bar(&at->main));
    if (opts & ATHR_ETA) widget_eta_create(widget_main_add_eta(&at->main));
    widget_main_setup(&at->main);

    atomic_store(&at->stop, false);

    if (!atomic_load_bool(&disable_thread))
        return thr_create(&at->thr, thread_start, at);

    return 0;
}

void athr_eat(struct athr *at, uint64_t amount)
{
    atomic_fetch_add_uint_fast64(&at->consumed, amount);
    if (atomic_load_bool(&disable_thread)) update(at);
}

void athr_stop(struct athr *at)
{
    atomic_store(&at->stop, true);
    update(at);
    athr_canvas_close(&at->main.canvas);
    thr_detach(&at->thr);
}

void athr_stop_wait(struct athr *at)
{
    atomic_store(&at->stop, true);
    update(at);
    athr_canvas_close(&at->main.canvas);
    thr_join(&at->thr);
}

void athr_disable_threading(bool disable)
{
    atomic_store(&disable_thread, disable);
}

int athr_sleep(unsigned milliseconds)
{
    return elapsed_sleep((unsigned)milliseconds);
}
