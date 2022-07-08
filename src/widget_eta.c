#include "athr/widget_eta.h"
#include "widget.h"
#include "widget_eta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_SPEED 0.000000001

#define xsnprintf(...) snprintf(__VA_ARGS__) < 0 ? abort() : (void)0

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif

static void human_readable(char *buff, unsigned sz, int secs)
{
    if (secs < 100)
        xsnprintf(buff, sz + 1, "%*ds", sz - 1, secs);
    else if (secs < 100 * 60)
        xsnprintf(buff, sz + 1, "%*dm %*ds", sz - 3 - 2, secs / 60, 2,
                  secs % 60);
    else if (secs < 48 * 3600)
        xsnprintf(buff, sz + 1, "%*dh %*dm", sz - 3 - 2, secs / 3600, 2,
                  (secs / 60) % 60);
    else if (secs < 100 * 86400)
        xsnprintf(buff, sz + 1, "%*dd %*dh", sz - 3 - 2, secs / 86400, 2,
                  (secs / 3600) % 24);
    else
        /* even (2^31-1)/86400 doesn't overflow eta->buff. */
        xsnprintf(buff, sz + 1, "%*dd", sz - 1, secs / 86400);
}

static void update(struct athr_widget *w, double consumed, double speed)
{
    unsigned const sz = ATHR_WIDGET_ETA_LEN;
    struct athr_widget_eta *eta = w->derived;
    eta->consumed = consumed;

    if (speed < MIN_SPEED)
        xsnprintf(eta->buff, sz + 1, "%*s", sz, " - eta - ");
    else
    {
        int secs = (int)((1 - eta->consumed) / speed);
        human_readable(eta->buff, sz, secs);
    }

    for (unsigned i = 0; i < sz; ++i)
        w->canvas.buff[i] = eta->buff[i];
}

static void finish(struct athr_widget *w, double total_elapsed)
{
    unsigned const sz = ATHR_WIDGET_ETA_LEN;
    struct athr_widget_eta *eta = w->derived;

    int secs = (int)total_elapsed;
    human_readable(eta->buff, sz, secs);

    for (unsigned i = 0; i < sz; ++i)
        w->canvas.buff[i] = eta->buff[i];
}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

static unsigned min_len(struct athr_widget const *w)
{
    (void)w;
    return ATHR_WIDGET_ETA_LEN;
}

static unsigned max_len(struct athr_widget const *w)
{
    (void)w;
    return ATHR_WIDGET_ETA_LEN;
}

static struct athr_widget_vtable const vtable = {update, finish, min_len,
                                                 max_len};

void __athr_widget_eta_create(struct athr_widget_eta *eta)
{
    widget_setup((struct athr_widget *)eta, &vtable);
}
