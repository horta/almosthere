#include "widget/line.h"
#include "canvas.h"
#include "terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>

struct line_data
{
    int             nwidgets;
    struct widget** widget;
};

void widget_line_finish(struct widget*);
void widget_line_update(struct widget*, double, double, double);
int  widget_line_get_min_length(struct widget*);
int  widget_line_get_max_length(struct widget*);
int  check_if_fit(int nwidgets, struct widget** widget);
int  widget_line_dist_len(int nwidgets, struct widget** widget, int length);

struct widget* widget_line_create(int nwidgets, struct widget** widget)
{
    struct line_data* l;
    int               i;
    struct widget*    w;

    if (nwidgets < 0) {
        fprintf(stderr, "nwidgets cannot be negative");
        exit(1);
    }

    if (!check_if_fit(nwidgets, widget)) {
        fprintf(stderr, "The widgets don't fit in the line widget.\n");
        return NULL;
    }

    w = malloc(sizeof(struct widget));
    l = malloc(sizeof(struct line_data));

    l->nwidgets = nwidgets;
    l->widget = malloc(((size_t)nwidgets) * sizeof(struct widget*));

    for (i = 0; i < nwidgets; ++i) {
        l->widget[i] = widget[i];
    }

    w->data = l;
    w->finish = widget_line_finish;
    w->update = widget_line_update;
    w->get_min_length = widget_line_get_min_length;
    w->get_max_length = widget_line_get_max_length;

    canvas_create(&w->canvas, widget_line_get_min_length(w));

    return w;
}

void widget_line_update(struct widget* widget, double consumed, double speed, double dlt)
{
    struct line_data* l = widget->data;
    int               i;
    int               base = 0;

    canvas_resize(&widget->canvas);
    canvas_clean(&widget->canvas);

    widget_line_dist_len(l->nwidgets, l->widget, widget->canvas.length - 1);

    for (i = 0; i < l->nwidgets; ++i) {
        l->widget[i]->canvas.buff = widget->canvas.buff + base;

        l->widget[i]->update(l->widget[i], consumed, speed, dlt);
        base += l->widget[i]->canvas.length;
    }
    canvas_draw(&widget->canvas);
}

void widget_line_finish(struct widget* widget)
{
    struct line_data* d = widget->data;
    int               i;

    for (i = 0; i < d->nwidgets; ++i) {
        d->widget[i]->finish(d->widget[i]);
    }

    free(d->widget);
    canvas_finish(&widget->canvas);
    free(d);
    free(widget);
}

int widget_line_dist_len(int nwidgets, struct widget** widget, int length)
{

    int i = 0;
    int j = 0;
    int part, len0, len1;

    for (i = 0; i < nwidgets; ++i) {
        len0 = widget[i]->get_min_length(widget[i]);
        len1 = widget[i]->get_max_length(widget[i]);
        widget[i]->canvas.length = len0;
        length -= len0;
        if (widget[i]->canvas.length < len1)
            j++;
    }

    while (j > 0 && length > 0) {
        part = length / j;
        if (part == 0)
            break;
        for (i = 0; i < nwidgets; ++i) {
            len0 = widget[i]->get_max_length(widget[i]);

            if (widget[i]->canvas.length == len0)
                continue;

            len1 = part;
            if (len1 >= len0 - widget[i]->canvas.length) {
                len1 = len0 - widget[i]->canvas.length;
                j--;
            }

            widget[i]->canvas.length += len1;
            length -= len1;
        }
    }

    for (i = 0; i < nwidgets; ++i) {
        if (j == 0 || length == 0)
            break;

        len0 = widget[i]->get_max_length(widget[i]);

        if (widget[i]->canvas.length == len0)
            continue;

        len1 = length;
        if (len1 >= len0 - widget[i]->canvas.length) {
            len1 = len0 - widget[i]->canvas.length;
            j--;
        }

        widget[i]->canvas.length += len1;
        length -= len1;
    }

    return length;
}

int widget_line_get_min_length(struct widget* widget)
{

    struct line_data* l = (struct line_data*)widget->data;
    int               s = 1;
    int               i;
    for (i = 0; i < l->nwidgets; ++i)
        s += l->widget[i]->get_min_length(l->widget[i]);
    return s;
}
int widget_line_get_max_length(struct widget* widget) { return ATHR_MAX_STR_LEN; }

int check_if_fit(int nwidgets, struct widget** widget)
{
    int i;
    int len = 0;
    for (i = 0; i < nwidgets; ++i) {
        len += widget[i]->get_min_length(widget[i]);
    }
    return len < widget_line_get_max_length(NULL);
}
