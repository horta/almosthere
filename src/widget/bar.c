#include "terminal/terminal.h"
#include "widget/bar.h"
#include <stdio.h>
#include <stdlib.h>

struct bar_data {
    double consumed;
};

struct widget *widget_bar_create(void) {

    struct bar_data *d = malloc(sizeof(struct bar_data));
    struct widget *w = malloc(sizeof(struct widget));
    d->consumed = -1.0;
    w->canvas = malloc(sizeof(struct canvas));
    w->data = d;
    w->finish = widget_bar_finish;
    w->update = widget_bar_update;
    w->get_min_length = widget_bar_get_min_length;
    w->get_max_length = widget_bar_get_max_length;
    return w;
}

void widget_bar_finish(struct widget *w) {
    free(w->data);
    free(w->canvas);
    free(w);
}

void bar_draw(struct bar_data *data, struct canvas *canvas) {

    int i;
    for (i = 0; i < (int)(data->consumed * canvas->length); ++i) {
        canvas->buff[i] = '.';
    }
}

void widget_bar_update(struct widget *w, double consumed, double speed,
                       double dlt) {
    struct bar_data *d = w->data;
    if (d->consumed == -1) {
        // First time this update is called.
        d->consumed = consumed;
    }
    d->consumed = speed * dlt + d->consumed;
    if (d->consumed > consumed)
        d->consumed = consumed;

    bar_draw(d, w->canvas);
}

int widget_bar_get_min_length(struct widget *widget) { return 20; }
int widget_bar_get_max_length(struct widget *widget) { return 20000; }
