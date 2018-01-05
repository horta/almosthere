#include "canvas.h"
#include "terminal/terminal.h"
#include "widget/line.h"
#include <stdio.h>
#include <stdlib.h>

struct line_data {
    int nwidgets;
    struct widget **widget;
};

int check_if_fit(int nwidgets, struct widget **widget) {
    int i;
    int len = 0;
    for (i = 0; i < nwidgets; ++i) {
        len += widget[i]->get_min_length(widget[i]);
    }
    return len < widget_line_get_min_length(NULL);
}

struct widget *widget_line_create(int nwidgets, struct widget **widget) {
    struct line_data *l;
    int i;
    struct widget *w;

    if (!check_if_fit(nwidgets, widget)) {
        fprintf(stderr, "The widgets don't fit under the line widget.\n");
        exit(0);
    }

    w = malloc(sizeof(struct widget));

    l = malloc(sizeof(struct line_data));
    l->nwidgets = nwidgets;
    l->widget = malloc(nwidgets * sizeof(struct widget *));

    for (i = 0; i < nwidgets; ++i) {
        l->widget[i] = widget[i];
    }

    w->data = l;
    w->finish = widget_line_finish;
    w->update = widget_line_update;
    w->get_min_length = widget_line_get_min_length;
    w->get_max_length = widget_line_get_max_length;

    w->canvas = canvas_create(widget_line_get_min_length(w));

    return w;
}

void widget_line_finish(struct widget *widget) {
    struct line_data *d = widget->data;
    int i;

    for (i = 0; i < d->nwidgets; ++i) {
        d->widget[i]->finish(d->widget[i]);
    }

    free(d->widget);
    canvas_finish(widget->canvas);
    free(d);
    free(widget);
}

void widget_line_update(struct widget *widget, double consumed, double speed,
                        double dlt) {
    struct line_data *l = widget->data;
    int i;
    int base = 0;

    canvas_clean(widget->canvas);
    widget->canvas->buff[widget->canvas->length - 1] = '\r';

    for (i = 0; i < l->nwidgets; ++i) {
        l->widget[i]->canvas->buff = widget->canvas->buff + base;

        // l->widget[i]->canvas->length = l->length[i];
        l->widget[i]->canvas->length =
            l->widget[i]->get_min_length(l->widget[i]);
        l->widget[i]->update(l->widget[i], consumed, speed, dlt);
        // base += l->length[i];
        base += l->widget[i]->get_min_length(l->widget[i]);
    }
    canvas_draw(widget->canvas);
}

int widget_line_get_min_length(struct widget *widget) { return 50; }
int widget_line_get_max_length(struct widget *widget) { return 20000; }
