#include "widget/perc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERC_LEN 5

struct perc_data {
    char str[PERC_LEN + 1];
    double consumed;
};

void widget_perc_update(struct widget *, double, double, double);
void widget_perc_finish(struct widget *);
int widget_perc_get_min_length(struct widget *);
int widget_perc_get_max_length(struct widget *);

struct widget *widget_perc_create(void) {

    struct perc_data *d = malloc(sizeof(struct perc_data));
    struct widget *w = malloc(sizeof(struct widget));

    w->data = d;
    w->finish = widget_perc_finish;
    w->update = widget_perc_update;
    w->get_min_length = widget_perc_get_min_length;
    w->get_max_length = widget_perc_get_max_length;

    return w;
}

void widget_perc_finish(struct widget *w) {
    free(w->data);
    free(w);
}

void widget_perc_update(struct widget *w, double consumed, double speed,
                        double dlt) {
    struct perc_data *d = w->data;
    int i, perc;

    perc = 0;
    d->consumed = consumed;

    if (d->consumed == 1.0)
        perc = 100;
    else
        perc = (int)(d->consumed * 100);

    snprintf(d->str, PERC_LEN + 1, " %3d%%", perc);

    for (i = 0; i < PERC_LEN; ++i) {
        w->canvas.buff[i] = d->str[i];
    }
}

int widget_perc_get_min_length(struct widget *w) { return PERC_LEN; }
int widget_perc_get_max_length(struct widget *w) { return PERC_LEN; }
