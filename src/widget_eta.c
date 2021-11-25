#include "athr/widget_eta.h"
#include "widget.h"
#include "widget_eta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_SPEED 0.000000001

#define MIN_SEC 60
#define HOUR_SEC 3600
#define DAY_SEC 86400
#define MONTH_SEC 2592000
#define YEAR_SEC 31104000

static inline unsigned year_sec(double s) { return (unsigned)(s / YEAR_SEC); }
static inline unsigned month_sec(double s) { return (unsigned)(s / MONTH_SEC); }
static inline unsigned day_sec(double s) { return (unsigned)(s / DAY_SEC); }
static inline unsigned hour_sec(double s) { return (unsigned)(s / HOUR_SEC); }
static inline unsigned min_sec(double s) { return (unsigned)(s / MIN_SEC); }

#define xsnprintf(...) snprintf(__VA_ARGS__) < 0 ? abort() : (void)0

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif

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

        /* Based on wget progress module */
        if (secs < 100)
            xsnprintf(eta->buff, sz + 1, "%*ds", sz - 1, secs);
        else if (secs < 100 * 60)
            xsnprintf(eta->buff, sz + 1, "%*dm %*ds", sz - 3 - 2, secs / 60, 2,
                      secs % 60);
        else if (secs < 48 * 3600)
            xsnprintf(eta->buff, sz + 1, "%*dh %*dm", sz - 3 - 2, secs / 3600,
                      2, (secs / 60) % 60);
        else if (secs < 100 * 86400)
            xsnprintf(eta->buff, sz + 1, "%*dd %*dh", sz - 3 - 2, secs / 86400,
                      2, (secs / 3600) % 24);
        else
            /* even (2^31-1)/86400 doesn't overflow eta->buff. */
            xsnprintf(eta->buff, sz + 1, "%*dd", sz - 1, secs / 86400);
    }

    for (unsigned i = 0; i < sz; ++i)
        w->canvas.buff[i] = eta->buff[i];
}

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

static unsigned min_len(struct athr_widget const *w)
{
    return ATHR_WIDGET_ETA_LEN;
}

static unsigned max_len(struct athr_widget const *w)
{
    return ATHR_WIDGET_ETA_LEN;
}

static struct athr_widget_vtable const vtable = {update, min_len, max_len};

void widget_eta_create(struct athr_widget_eta *eta)
{
    widget_setup((struct athr_widget *)eta, &vtable);
}
