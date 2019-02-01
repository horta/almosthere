#include "widget/eta.h"
#include "snprintf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ETA_SIZE 9
#define MIN_SPEED 0.000000001

#define MIN_SEC 60
#define HOUR_SEC 3600
#define DAY_SEC 86400
#define MONTH_SEC 2592000
#define YEAR_SEC 31104000

struct eta_data
{
    char str[ETA_SIZE + 1];
    double consumed;
};

void widget_eta_update(struct widget *, double, double, double);
void widget_eta_finish(struct widget *);
int widget_eta_get_min_length(struct widget *);
int widget_eta_get_max_length(struct widget *);

struct widget *widget_eta_create(void)
{

    struct eta_data *d = malloc(sizeof(struct eta_data));
    struct widget *w = malloc(sizeof(struct widget));

    w->data = d;
    w->finish = widget_eta_finish;
    w->update = widget_eta_update;
    w->get_min_length = widget_eta_get_min_length;
    w->get_max_length = widget_eta_get_max_length;

    return w;
}

void widget_eta_finish(struct widget *w)
{
    free(w->data);
    free(w);
}

void widget_eta_update(struct widget *w, double consumed, double speed, double dlt)
{
    struct eta_data *d = w->data;
    int i;
    double eta_sec;

    d->consumed = consumed;

    if (d->consumed == 1.0)
        eta_sec = 0.0;

    if (speed < MIN_SPEED) {
        snprintf(d->str, ETA_SIZE + 1, "%*s", ETA_SIZE, " - eta - ");
    } else {
        eta_sec = (1 - d->consumed) / speed;

        if (eta_sec >= YEAR_SEC)
            snprintf(d->str, ETA_SIZE + 1, "%*d years", ETA_SIZE - 6,
                     (int)(eta_sec / YEAR_SEC));
        else if (eta_sec >= MONTH_SEC)
            snprintf(d->str, ETA_SIZE + 1, "%*d months", ETA_SIZE - 7,
                     (int)(eta_sec / MONTH_SEC));
        else if (eta_sec >= DAY_SEC)
            snprintf(d->str, ETA_SIZE + 1, "%*d days", ETA_SIZE - 5,
                     (int)(eta_sec / DAY_SEC));
        else if (eta_sec >= HOUR_SEC)
            snprintf(d->str, ETA_SIZE + 1, "%*d hours", ETA_SIZE - 6,
                     (int)(eta_sec / HOUR_SEC));
        else if (eta_sec >= MIN_SEC)
            snprintf(d->str, ETA_SIZE + 1, "%*d mins", ETA_SIZE - 5,
                     (int)(eta_sec / MIN_SEC));
        else
            snprintf(d->str, ETA_SIZE + 1, "%*d secs", ETA_SIZE - 5, (int)eta_sec);
    }

    for (i = 0; i < ETA_SIZE; ++i) {
        w->canvas.buff[i] = d->str[i];
    }
}

int widget_eta_get_min_length(struct widget *w) { return ETA_SIZE; }
int widget_eta_get_max_length(struct widget *w) { return ETA_SIZE; }
