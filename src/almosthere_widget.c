#include "almosthere_widget.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

struct line_data {
    int nwidgets;
    struct almosthere_widget **widget;
    int *start;
    int *length;
};
//
struct almosthere_widget *
almosthere_widget_line_create(int nwidgets, struct almosthere_widget **widget,
                              int *start, int *length) {
    struct line_data *l;
    int i;
    struct almosthere_widget *w;
    w = malloc(sizeof(struct almosthere_widget));

    l = malloc(sizeof(struct line_data));
    l->nwidgets = nwidgets;
    l->widget = malloc(nwidgets * sizeof(struct almosthere_widget *));
    l->start = malloc(nwidgets * sizeof(int));
    l->length = malloc(nwidgets * sizeof(int));

    for (i = 0; i < nwidgets; ++i) {
        l->widget[i] = widget[i];
        l->start[i] = start[i];
        l->length[i] = length[i];
    }

    w->data = l;
    w->finish = almosthere_widget_line_finish;
    w->update = almosthere_widget_line_update;

    return w;
}

void almosthere_widget_line_finish(struct almosthere_widget *widget) {
    struct line_data *d = widget->data;
    int i;

    for (i = 0; i < d->nwidgets; ++i) {
        d->widget[i]->finish(d->widget[i]);
    }

    free(d->widget);
    free(d->start);
    free(d->length);
}

void almosthere_widget_line_update(struct almosthere_widget *widget,
                                   double consumed, double speed, double dlt) {
    struct line_data *w = widget->data;
    int i;

    for (i = 0; i < w->nwidgets; ++i) {
        w->widget[i]->update(w->widget[i], consumed, speed, dlt);
    }
    fputc('\r', stderr);
    fflush(stderr);
}

struct bar_data {
    double consumed;
};

struct almosthere_widget *almosthere_widget_bar_create(void) {

    struct bar_data *d = malloc(sizeof(struct bar_data));
    struct almosthere_widget *w = malloc(sizeof(struct almosthere_widget));
    d->consumed = -1.0;
    w->data = d;
    w->finish = almosthere_widget_bar_finish;
    w->update = almosthere_widget_bar_update;
    return w;
}

void almosthere_widget_bar_finish(struct almosthere_widget *w) {
    free(w->data);
    free(w);
}

void bar_draw(struct bar_data *data) {

    int i;
    unsigned length = almosthere_get_term_width();
    for (i = 0; i < (int)(data->consumed * length); ++i) {
        fputc('.', stderr);
    }
    fputc('\r', stderr);
    fflush(stderr);
}

void almosthere_widget_bar_update(struct almosthere_widget *w, double consumed,
                                  double speed, double dlt) {
    struct bar_data *d = w->data;
    if (d->consumed == -1) {
        // First time this update is called.
        d->consumed = consumed;
    }
    d->consumed = speed * dlt + d->consumed;
    if (d->consumed > consumed)
        d->consumed = consumed;

    bar_draw(d);
}
