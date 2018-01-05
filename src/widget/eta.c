#include "widget/eta.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int ETA_SIZE = 9;
const double MIN_SPEED = 0.000001;

struct eta_data {
    char str[ETA_SIZE + 1];
    double consumed;
};

void widget_eta_update(struct widget *, double, double, double);
void widget_eta_finish(struct widget *);
int widget_eta_get_min_length(struct widget *);
int widget_eta_get_max_length(struct widget *);

struct widget *widget_eta_create(void) {

    struct eta_data *d = malloc(sizeof(struct eta_data));
    struct widget *w = malloc(sizeof(struct widget));

    w->data = d;
    w->finish = widget_eta_finish;
    w->update = widget_eta_update;
    w->get_min_length = widget_eta_get_min_length;
    w->get_max_length = widget_eta_get_max_length;

    return w;
}

void widget_eta_finish(struct widget *w) {
    free(w->data);
    free(w);
}

void widget_eta_update(struct widget *w, double consumed, double speed,
                       double dlt) {
    struct eta_data *d = w->data;
    int i;
    double eta_sec;

    if (d->consumed == -1.0) {
        // First time this update is called.
        d->consumed = consumed;
    }
    d->consumed = speed * dlt + d->consumed;
    if (d->consumed > consumed)
        d->consumed = consumed;

    if (d->consumed == 1.0)
        eta_sec = 0.0;

    if (speed < MIN_SPEED) {
        snprintf(d->str, ETA_SIZE + 1, "%*s", ETA_SIZE, " ");
    } else {
        eta_sec = (1 - d->consumed) / speed;
        snprintf(d->str, ETA_SIZE + 1, "%*s", ETA_SIZE, " ");
    }

    // snprintf(d->str + 1, 5, "%3d%%", perc);

    for (i = 0; i < ETA_SIZE; ++i) {
        w->canvas.buff[i] = d->str[i];
    }
}

int widget_eta_get_min_length(struct widget *w) { return ETA_SIZE; }
int widget_eta_get_max_length(struct widget *w) { return ETA_SIZE; }
