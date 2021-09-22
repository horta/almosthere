#include "athr/athr.h"
#include "athr.h"
#include "athr/widget/main.h"
#include "ema.h"
#include "thr.h"
#include "widget/bar.h"
#include "widget/eta.h"
#include "widget/text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* How often to update, in seconds. */
static double const TIMESTEP = 1.0 / 30.0;

static atomic_bool disable_threading = false;

static void setup_widget(struct athr_widget_main *main, const char *desc,
                         enum athr_option opts)
{
    widget_main_create(main);
    widget_text_create(widget_main_setup_text(main), desc);
    if (opts & ATHR_PERC) widget_perc_setup(widget_main_add_perc(main));
    if (opts & ATHR_BAR) widget_bar_setup(widget_main_add_bar(main));
    if (opts & ATHR_ETA) widget_eta_setup(widget_main_add_eta(main));
    widget_main_setup(main);
}

static void lock(struct athr *at)
{
    while (atomic_flag_test_and_set(&at->lock))
        /* spin until the lock is acquired */;
}

static void unlock(struct athr *at) { atomic_flag_clear(&at->lock); }

static void update(struct athr *at)
{
    lock(at);
    double elapsed = elapsed_stop(&at->elapsed);
    ema_add(&at->speed, (float)elapsed);

    double ratio = atomic_load_ul(&at->consumed) / ((double)at->total);
    at->main.super.vtable->update(&at->main.super, ratio, ema_get(&at->speed),
                                  elapsed);

    elapsed_start(&at->elapsed);
    unlock(at);
}

static void thread_start(void *args)
{
    struct athr *at = (struct athr *)args;
    while (!atomic_load_bool(&at->stop) &&
           !atomic_load_bool(&disable_threading))
    {
        update(at);
        elapsed_sleep(TIMESTEP);
    }
    thr_exit();
}

enum athr_rc athr_start(struct athr *at, unsigned long total, const char *desc,
                        enum athr_option opts)
{
    if (desc == NULL) desc = "";
    at->total = total;
    at->consumed = 0;
    at->speed = ATHR_EMA_INIT;
    elapsed_start(&at->elapsed);

    at->opts = opts;
    setup_widget(&at->main, desc, opts);

    atomic_store(&at->stop, false);

    if (!atomic_load_bool(&disable_threading))
        return thr_create(&at->thr, thread_start, at);

    return ATHR_SUCCESS;
}

void athr_eat(struct athr *at, unsigned long amount)
{
    atomic_fetch_add_ul(&at->consumed, amount);
    if (atomic_load_bool(&disable_threading)) update(at);
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
    atomic_store(&disable_threading, disable);
}
