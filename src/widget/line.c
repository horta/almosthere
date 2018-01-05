#include "terminal/terminal.h"
#include "widget/line.h"
#include <stdio.h>
#include <stdlib.h>

struct line_data {
    int nwidgets;
    struct widget **widget;
    int *start;
    int *length;
};

struct widget *widget_line_create(int nwidgets, struct widget **widget,
                                  int *start, int *length) {
    struct line_data *l;
    int i;
    struct widget *w;
    w = malloc(sizeof(struct widget));

    l = malloc(sizeof(struct line_data));
    l->nwidgets = nwidgets;
    l->widget = malloc(nwidgets * sizeof(struct widget *));
    l->start = malloc(nwidgets * sizeof(int));
    l->length = malloc(nwidgets * sizeof(int));

    for (i = 0; i < nwidgets; ++i) {
        l->widget[i] = widget[i];
        l->start[i] = start[i];
        l->length[i] = length[i];
    }

    w->data = l;
    w->finish = widget_line_finish;
    w->update = widget_line_update;

    return w;
}

void widget_line_finish(struct widget *widget) {
    struct line_data *d = widget->data;
    int i;

    for (i = 0; i < d->nwidgets; ++i) {
        d->widget[i]->finish(d->widget[i]);
    }

    free(d->widget);
    free(d->start);
    free(d->length);
}

void widget_line_update(struct widget *widget, double consumed, double speed,
                        double dlt) {
    struct line_data *w = widget->data;
    int i;
    int acc = 0;
    for (i = 0; i < w->nwidgets; ++i) {
        w->widget[i]->canvas.buff = widget->canvas.buff + acc;
        w->widget[i]->canvas.length = w->length[i];
        w->widget[i]->update(w->widget[i], consumed, speed, dlt);
        acc += w->length[i];
    }
    fputc('\r', stderr);
    fflush(stderr);
}
