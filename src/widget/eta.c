#include "athr/widget/eta.h"
#include "widget/widget.h"

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

static void update(struct athr_widget *w, double consumed, double speed,
                   double dlt)
{
    unsigned const sz = ATHR_WIDGET_ETA_SIZE;
    struct athr_widget_eta *eta = w->derived;
    eta->consumed = consumed;

    if (speed < MIN_SPEED)
        snprintf(eta->str, sz + 1, "%*s", sz, " - eta - ");
    else
    {
        double sec = (1 - eta->consumed) / speed;

        if (sec >= YEAR_SEC)
            snprintf(eta->str, sz + 1, "%*d years", sz - 6, year_sec(sec));
        else if (sec >= MONTH_SEC)
            snprintf(eta->str, sz + 1, "%*d months", sz - 7, month_sec(sec));
        else if (sec >= DAY_SEC)
            snprintf(eta->str, sz + 1, "%*d days", sz - 5, day_sec(sec));
        else if (sec >= HOUR_SEC)
            snprintf(eta->str, sz + 1, "%*d hours", sz - 6, hour_sec(sec));
        else if (sec >= MIN_SEC)
            snprintf(eta->str, sz + 1, "%*d mins", sz - 5, min_sec(sec));
        else
            snprintf(eta->str, sz + 1, "%*d secs", sz - 5, (unsigned)sec);
    }

    for (unsigned i = 0; i < sz; ++i)
        w->canvas.buff[i] = eta->str[i];
}

static unsigned min_size(struct athr_widget const *w)
{
    return ATHR_WIDGET_ETA_SIZE;
}

static unsigned max_size(struct athr_widget const *w)
{
    return ATHR_WIDGET_ETA_SIZE;
}

static struct athr_widget_vtable const vtable = {update, min_size, max_size};

void widget_eta_create(struct athr_widget_eta *eta)
{
    widget_setup((struct athr_widget *)eta, &vtable);
}
