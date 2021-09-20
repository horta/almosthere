#define ATHR_DISABLE_DEPRECATED_API 1
#include "athr/athr.h"
#include "athr/widget/main.h"
#include "ema.h"
#include "terminal/terminal.h"
#include "thr.h"
#include "widget/bar.h"
#include "widget/eta.h"
#include "widget/text.h"
#include <stdio.h>
#include <stdlib.h>

/* How often to update, in seconds. */
static double const TIMESTEP = 1.0 / 30.0;

static void setup_widget(struct athr_widget_main *main, const char *desc,
                         enum athr_option opts)
{
    widget_main_setup(main);
    widget_text_create(widget_main_setup_text(main), desc);
    if (opts & ATHR_PERC) widget_perc_setup(widget_main_setup_perc(main));
    if (opts & ATHR_BAR) widget_bar_create(widget_main_setup_bar(main));
    if (opts & ATHR_ETA) widget_eta_create(widget_main_setup_eta(main));
    widget_main_assert_that_fits(main);
}

static void update(struct athr *at)
{
    while (atomic_flag_test_and_set(&at->lock))
        ;
    double elapsed = elapsed_stop(&at->elapsed);
    ema_add(&at->speed, (float)elapsed);

    double ratio = ((double)at->consumed) / ((double)at->total);
    at->main.super.vtable->update(&at->main.super, ratio, ema_get(&at->speed),
                                  elapsed);

    elapsed_start(&at->elapsed);
    atomic_flag_clear(&at->lock);
}

static void thread_start(void *args)
{
    struct athr *at = (struct athr *)args;
    while (!at->stop)
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
    if (thr_create(&at->thr, thread_start, at)) return ATHR_FAILURE;
    return ATHR_SUCCESS;
}

void athr_eat(struct athr *at, unsigned long amount)
{
    assert(at->consumed + amount <= at->total);
    at->consumed += amount;
}

void athr_stop(struct athr *at)
{
    atomic_store(&at->stop, true);
    update(at);
    elapsed_sleep(0.001);
}
