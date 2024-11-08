#include "athr.h"
#include "athr_elapsed.h"
#include "athr_ema.h"
#include "athr_logger.h"
#include "athr_thread.h"
#include "athr_widget_bar.h"
#include "athr_widget_eta.h"
#include "athr_widget_main.h"
#include "athr_widget_perc.h"
#include "athr_widget_text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(ATHR_OS_WIN32)
#include "athr_ovs_atomic_msvc.h"
#elif defined(ATHR_OS_UNIX)
#include "athr_ovs_atomic_posix.h"
#endif

#define error(msg) athr_logger_error(athr_logger_format(msg))

static atomic_bool disable_thread = false;

static void lock(struct athr *at)
{
    while (atomic_flag_test_and_set(&at->lock))
        /* spin until the lock is acquired */;
}

static void unlock(struct athr *x) { atomic_flag_clear(&x->lock); }

static void update(struct athr *x)
{
    lock(x);
    uint_fast64_t consumed = atomic_load_uint_fast64(&x->consumed);
    if (consumed > x->total) consumed = x->total;
    if (consumed == x->last_consumed) goto cleanup;
    uint_fast64_t delta = consumed - x->last_consumed;
    x->last_consumed = consumed;

    if (athr_elapsed_stop(x->elapsed)) error("failed to elapsed_stop");

    double seconds = ((double)athr_elapsed_milliseconds(x->elapsed)) / 1000.;
    double progress = ((double)delta) / ((double)x->total);

    if (progress < 0.005f && x->timestep < ATHR_TIMESTEP_LIMIT)
    {
        x->timestep += ATHR_TIMESTEP;
        if (x->timestep > ATHR_TIMESTEP_LIMIT)
            x->timestep = ATHR_TIMESTEP_LIMIT;
    }

    athr_ema_add(&x->speed, progress / seconds);

    double consumed_fraction = ((double)consumed) / ((double)x->total);
    x->main.super.vtable->update(&x->main.super, consumed_fraction,
                                  athr_ema_get(&x->speed));

    if (athr_elapsed_start(x->elapsed)) error("failed to elapsed_start");
cleanup:
    unlock(x);
}

static void thread_start(void *args)
{
    struct athr *at = (struct athr *)args;
    while (!atomic_load_bool(&at->stop) && !atomic_load_bool(&disable_thread))
    {
        update(at);
        if (athr_sleep(at->timestep)) error("failed to sleep");
    }
}

int athr_start(struct athr *x, uint64_t total, const char *desc,
               enum athr_option opts)
{
    if (desc == NULL) desc = "";
    x->timestep = ATHR_TIMESTEP;
    x->total = total;
    x->consumed = 0;
    x->last_consumed = 0;
    x->speed = ATHR_EMA_INIT;
    x->elapsed = athr_elapsed_new();
    x->total_elapsed = athr_elapsed_new();
    if (!x->elapsed || !x->total_elapsed)
    {
        athr_elapsed_del(x->elapsed);
        athr_elapsed_del(x->total_elapsed);
        x->elapsed = NULL;
        x->total_elapsed = NULL;
        error("failed to allocate elapsed struct");
        return 1;
    }
    if (athr_elapsed_start(x->elapsed) || athr_elapsed_start(x->total_elapsed))
    {
        athr_elapsed_del(x->elapsed);
        athr_elapsed_del(x->total_elapsed);
        x->elapsed = NULL;
        x->total_elapsed = NULL;
        error("failed to elapsed_start");
        return 1;
    }

    x->opts = opts;
    athr_widget_main_create(&x->main);
    athr_widget_text_create(athr_widget_main_add_text(&x->main), desc);
    if (opts & ATHR_PERC)
        athr_widget_perc_create(athr_widget_main_add_perc(&x->main));
    if (opts & ATHR_BAR)
        athr_widget_bar_create(athr_widget_main_add_bar(&x->main));
    if (opts & ATHR_ETA)
        athr_widget_eta_create(athr_widget_main_add_eta(&x->main));
    athr_widget_main_setup(&x->main);

    atomic_store(&x->stop, false);

    if (!atomic_load_bool(&disable_thread))
    {
        int rc = athr_thread_create(&x->thr, thread_start, x);
        if (rc)
        {
            athr_elapsed_del(x->elapsed);
            athr_elapsed_del(x->total_elapsed);
            x->elapsed = NULL;
            x->total_elapsed = NULL;
        }
        return rc;
    }

    return 0;
}

void athr_eat(struct athr *x, uint64_t amount)
{
    atomic_fetch_add_uint_fast64(&x->consumed, amount);
    if (atomic_load_bool(&disable_thread)) update(x);
}

void athr_stop(struct athr *x)
{
    atomic_store(&x->stop, true);
    update(x);
    athr_thread_join(&x->thr);

    if (athr_elapsed_stop(x->total_elapsed)) error("failed to elapsed_stop");

    double seconds = ((double)athr_elapsed_milliseconds(x->total_elapsed)) / 1000.;
    x->main.super.vtable->finish(&x->main.super, seconds);
    athr_canvas_close(&x->main.canvas);
    athr_elapsed_del(x->elapsed);
    athr_elapsed_del(x->total_elapsed);
    x->elapsed = NULL;
    x->total_elapsed = NULL;
}

void athr_stop_wait(struct athr *at)
{
    atomic_store(&at->stop, true);
    update(at);
    athr_thread_join(&at->thr);

    athr_canvas_close(&at->main.canvas);
}

void athr_disable_threading(bool disable)
{
    atomic_store(&disable_thread, disable);
}

int athr_sleep(unsigned milliseconds)
{
    return athr_elapsed_sleep((unsigned)milliseconds);
}
